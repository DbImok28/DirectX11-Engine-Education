#pragma once
#include "Mesh.hpp"

using namespace DirectX;

class Model
{
public:
	bool Initialize(std::string path, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_VertexShader>& cb_vs_VertexShader);
	void Draw(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionMatrix);
private:
	bool LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	TextureStorageType DetermineTextureStorageType(const aiScene* scene, aiMaterial* pMaterial, unsigned int index, aiTextureType textureType);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* scene);
	int GetTextureIndex(aiString* pStr);

	std::vector<Mesh> meshes;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ConstantBuffer<CB_VS_VertexShader>* cb_vs_VertexShader;
	std::string directory = "";
};