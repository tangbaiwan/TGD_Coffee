#pragma once

#include "CoreMinimal.h"
#include "Model/GlobalInfo.h"
#include "Model/Chunk.h"
#include "Model/Block.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <mutex>
#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "InstBlock.generated.h"

static bool GameEnd = false;

//��չ��
static TSet<uint64> domains;
//�Ѵ��ڵ�·
static TSet<uint64> roads;
/**
 * ����Ⱥ�����Է���
 */
enum class BiomeType {
	None = 0,
	Snow = 1,
	Green = 2,
	Dry = 3,
	Stone = 4,
	Desert = 5
};

UCLASS()
class TERRIANGENERATEDEMO_API AInstBlock : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AInstBlock();


	TArray<uint64> AllIndex;

	TArray<TPair<float, FVector>> CurCreateChunkList;
	TArray<FTransform> CurCreatePosList;
	TArray<uint64> CurCreateIndexList;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
		UInstancedStaticMeshComponent* InstancedStaticMesh;
		//TObjectPtr<UInstancedStaticMeshComponent> InstancedStaticMesh;

	static FCriticalSection MyMutex;

	//����LoadRadius
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BXS")
		int32 LoadRadius = 5;

	//����λ��
	UPROPERTY(VisibleAnywhere)
		FVector2D ChunksCenterPosition;


	APlayerController* PlayerController = nullptr;
	FVector PCLocation = FVector::ZeroVector;
	FRotator PCRotation = FRotator::ZeroRotator;

	int32 Cur_CreateNums = -1;
	int32 ChunkSize = LoadRadius * 2 - 1;
	int32 Center = LoadRadius - 1;

	int32 DisplayRadius = LoadRadius - 1;
	int32 DisplaySize = DisplayRadius * 2 - 1;
	int32 DisplayCenter = DisplayRadius - 1;

	TArray<uint64> AllCreateindex;
	//ȫ��Chunk
	TMap<uint64, Chunk> AllChunk;
	//������ʾ��Chunk
	TSet<Chunk*> Chunks2Display;
	//ȫ��Block
	//TMap<uint64,ABlock*> ABlocks;

	AInstBlock* BBlocks;
	//ȫ����Ϣ
	GlobalInfo Info;



	//��չ���ʼ���ĵ�
	FVector2D startPoint;
	//���н����ſڵ�
	TArray<FVector2D> buildingPos;

	//��Ҷģ�壨������Ҷ�׶��ã�
	bool leavesTemplate[4][5][5] = {
{   {0,0,0,0,0},
	{0,0,1,0,0},
	{0,1,1,1,0},
	{0,0,1,0,0},
	{0,0,0,0,0}},

{   {0,0,0,0,0},
	{0,1,1,1,0},
	{0,1,1,1,0},
	{0,1,1,1,0},
	{0,0,0,0,0}},

{   {0,1,1,1,0},
	{1,1,1,1,1},
	{1,1,1,1,1},
	{1,1,1,1,1},
	{0,1,1,1,0}},

{   {1,1,1,1,1},
	{1,1,1,1,1},
	{1,1,1,1,1},
	{1,1,1,1,1},
	{1,1,1,1,1}}
	};

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//�������������ң�����λ��
	UFUNCTION(BlueprintCallable, Category = "Func")
		void SetCameraLoaction(FVector location);

	//������ΧChunk
	UFUNCTION(BlueprintCallable, Category = "Func")
		void UpdateChunks();

	UFUNCTION(BlueprintCallable, Category = "Func")
		void AsyncUpdateChunks(float SleepValue);

	UFUNCTION(BlueprintCallable, Category = "Func")
		bool Testbool(int32 SleepValue, TArray<int32>& GlobalBlocksID);


	//------------------------����Chunk---------------------
	//����Chunk
	void LoadChunk(Chunk& chunk);

	//�Ƿ���Ҫ����chunk
	bool NeedLoadChunk(FVector2D chunkPosition);

	//���ص��η���ID
	void LoadTerrianBlocksID(Chunk& chunk);

	//���ɽ�������
	void GenerateBuildingBlocks();

	//���㷽��ID
	int32 CaculateBlockID(Chunk& chunk, int32 i, int32 j, int32 k);

	//------------------------��ʾChunk---------------------
	//��ʾChunk
	void DisplayChunk();

	void Get_CreateNums(int32 Cur_CreateNum);


	void Set_CreateNums();



	//����Block Actor
	bool CreateBlock(int32 id, FVector pos);


	//����Building Actor
	bool CreateBuilding(int32 id, int32 rotate, FVector pos);

	void GenerateBuilding(Chunk& chunk, GlobalInfo& info);

	void DevelopeDomains(Chunk& chunk, GlobalInfo& info);

	void PlaceBuildings(Chunk& chunk, GlobalInfo& info);

	void PlacePaths(Chunk& chunk, GlobalInfo& info);

	


	bool PlaceOneBuilding(GlobalInfo& info, int32 x, int32 y, int32 buildingIndex, int32 rotate);


	//ʪ��
	void GenerateHumidity(Chunk& chunk);

	//����Ⱥ�������
	void GenerateBiome(Chunk& chunk);

	//�����¶�
	void GenerateTemperature(Chunk& chunk);

	//���ɶ�Ѩ
	void GenerateCave(Chunk& chunk, GlobalInfo& info);

	//���ɸ߶�
	void GenerateHeight(Chunk& chunk, GlobalInfo& info);

	//������
	void GeneratePlant(Chunk& chunk, GlobalInfo& info);
	//���ɻ�
	bool GenerateFlower(Chunk& chunk, GlobalInfo& info, int32 i, int32 j, int32 cystalSize);
	//������
	bool GenerateTree(Chunk& chunk, GlobalInfo& info, int32 i, int32 j, int32 cystalSize);
	//������Ҷ
	void GenerateLeaves(Chunk& chunk, GlobalInfo& info, int32 x, int32 y, int32 height, int32 radius, int32 targetLeafID);

	
};
