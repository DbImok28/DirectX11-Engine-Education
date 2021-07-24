#include "Model.hpp"

bool Model::Initialize(std::string path, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_VertexShader>& cb_vs_VertexShader)
{
	this->device = device;
	this->deviceContext = deviceContext;
	this->cb_vs_VertexShader = &cb_vs_VertexShader;
	
	try
	{
		if (!LoadModel(path))
			return false;
	}
	catch (COMException exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}
	return true;
}

void Model::Draw(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionMatrix)
{
	cb_vs_VertexShader->data.mat = worldMatrix * viewProjectionMatrix;
	cb_vs_VertexShader->data.mat = XMMatrixTranspose(cb_vs_VertexShader->data.mat);
	//if (!cb_vs_VertexShader->ApplyChanges()) return;
	cb_vs_VertexShader->ApplyChanges();
	deviceContext->VSSetConstantBuffers(0, 1, cb_vs_VertexShader->GetAddressOf());

	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw();
	}

}

bool Model::LoadModel(const std::string& path)
{
	directory = Paths::GetDirectoryFromPath(path);
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(
		path, 
		aiProcess_Triangulate | aiProcess_ConvertToLeftHanded
	);
	if (pScene == nullptr)
		return false;

	ProcessNode(pScene->mRootNode, pScene);
	return true;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}
	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.pos.x = static_cast<float>(mesh->mVertices[i].x);
		vertex.pos.y = static_cast<float>(mesh->mVertices[i].y);
		vertex.pos.z = static_cast<float>(mesh->mVertices[i].z);
		if (mesh->mTextureCoords[0])
		{
			vertex.texCoord.x = static_cast<float>(mesh->mTextureCoords[0][i].x);
			vertex.texCoord.y = static_cast<float>(mesh->mTextureCoords[0][i].y);
		}
		vertices.push_back(vertex);
	}
	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	std::vector<Texture> textures;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture> diffuseTextures = LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, scene);
	textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

	return Mesh(device, deviceContext, vertices, indices, textures);
}

TextureStorageType Model::DetermineTextureStorageType(const aiScene* scene, aiMaterial* pMaterial, unsigned int index, aiTextureType textureType)
{
	if (pMaterial->GetTextureCount(textureType) == 0)
		return TextureStorageType::None;
	aiString path;
	pMaterial->GetTexture(textureType, index, &path);
	std::string texturePath = path.C_Str();
	if (texturePath[0] == '*')
	{
		if (scene->mTextures[0]->mHeight == 0)
		{
			return TextureStorageType::EmbeddedIndexCompressed;
		}
		else
		{
			assert("no support EmbeddedIndexNonCompressed" && 0);
			return TextureStorageType::EmbeddedIndexNonCompressed;
		}
	}
	if (auto pTex = scene->GetEmbeddedTexture(texturePath.c_str()))
	{
		if (pTex->mHeight == 0)
		{
			return TextureStorageType::EmbeddedCompressed;
		}
		else
		{
			assert("no support EmbeddedNonCompressed" && 0);
			return TextureStorageType::EmbeddedNonCompressed;
		}
	}
	if (texturePath.find('.') != std::string::npos)
	{
		return TextureStorageType::Disk;
	}
	return TextureStorageType::None;
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* scene)
{
	std::vector<Texture> materialTextures;
	TextureStorageType storeType = TextureStorageType::Invalid;
	unsigned int textureCount = pMaterial->GetTextureCount(textureType);
	if (textureCount == 0) // Color?
	{
		storeType = TextureStorageType::None;
		aiColor3D aiColor(0.0f, 0.0f, 0.0f);
		switch (textureType)
		{
		case aiTextureType_DIFFUSE:
			pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
			if (aiColor.IsBlack())
			{
				materialTextures.push_back(Texture(device, Colors::UnloadedTextureColor, textureType));
				return materialTextures;
			}
			materialTextures.push_back(Texture(device, Color(aiColor.r * 255, aiColor.g * 255, aiColor.b * 255), textureType));
			return materialTextures;
		}
	}
	else // Texture!
	{
		for (UINT i = 0; i < textureCount; i++)
		{
			aiString path;
			pMaterial->GetTexture(textureType, i, &path);
			storeType = DetermineTextureStorageType(scene, pMaterial, i, textureType);
			switch (storeType)
			{
			case TextureStorageType::Invalid:
				break;
			case TextureStorageType::None:
				break;
			case TextureStorageType::EmbeddedIndexCompressed:
			{
				int index = GetTextureIndex(&path);
				Texture embeddedIndexedTexture(
					device,
					reinterpret_cast<uint8_t*>(scene->mTextures[index]->pcData),
					scene->mTextures[index]->mWidth,
					textureType
				);
				materialTextures.push_back(embeddedIndexedTexture);
				break;
			}
			case TextureStorageType::EmbeddedIndexNonCompressed:
				break;
			case TextureStorageType::EmbeddedCompressed:
			{
				const aiTexture* pTexture = scene->GetEmbeddedTexture(path.C_Str());
				Texture embeddedTexture(
					device,
					reinterpret_cast<uint8_t*>(pTexture->pcData),
					pTexture->mWidth,
					textureType
				);
				materialTextures.push_back(embeddedTexture);
				break;
			}
			case TextureStorageType::EmbeddedNonCompressed:
				break;
			case TextureStorageType::Disk:
			{
				std::string filename = directory + '\\' + path.C_Str();
				Texture diskTexture(device, filename, textureType);
				materialTextures.push_back(diskTexture);
				break;
			}
			default:
				break;
			}
		}
	}
	if (materialTextures.size() == 0)
	{
		materialTextures.push_back(Texture(device, Colors::UnhandledTextureColor, textureType));
	}
	return materialTextures;
}

// read "*0", "*1", "*2" ...
int Model::GetTextureIndex(aiString* pStr)
{
	assert(pStr->length >= 2);
	return atoi(&pStr->C_Str()[1]);
}
