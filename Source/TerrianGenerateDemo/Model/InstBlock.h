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

//发展域
static TSet<uint64> domains;
//已存在道路
static TSet<uint64> roads;
/**
 * 生物群落属性分类
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

	//设置LoadRadius
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BXS")
		int32 LoadRadius = 5;

	//中心位置
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
	//全部Chunk
	TMap<uint64, Chunk> AllChunk;
	//用于显示的Chunk
	TSet<Chunk*> Chunks2Display;
	//全部Block
	//TMap<uint64,ABlock*> ABlocks;

	AInstBlock* BBlocks;
	//全局信息
	GlobalInfo Info;



	//发展域初始中心点
	FVector2D startPoint;
	//所有建筑门口点
	TArray<FVector2D> buildingPos;

	//树叶模板（生成树叶阶段用）
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

	//设置摄像机（玩家）中心位置
	UFUNCTION(BlueprintCallable, Category = "Func")
		void SetCameraLoaction(FVector location);

	//更新周围Chunk
	UFUNCTION(BlueprintCallable, Category = "Func")
		void UpdateChunks();

	UFUNCTION(BlueprintCallable, Category = "Func")
		void AsyncUpdateChunks(float SleepValue);

	UFUNCTION(BlueprintCallable, Category = "Func")
		bool Testbool(int32 SleepValue, TArray<int32>& GlobalBlocksID);


	//------------------------加载Chunk---------------------
	//载入Chunk
	void LoadChunk(Chunk& chunk);

	//是否需要加载chunk
	bool NeedLoadChunk(FVector2D chunkPosition);

	//加载地形方块ID
	void LoadTerrianBlocksID(Chunk& chunk);

	//生成建筑方块
	void GenerateBuildingBlocks();

	//计算方块ID
	int32 CaculateBlockID(Chunk& chunk, int32 i, int32 j, int32 k);

	//------------------------显示Chunk---------------------
	//显示Chunk
	void DisplayChunk();

	void Get_CreateNums(int32 Cur_CreateNum);


	void Set_CreateNums();



	//创建Block Actor
	bool CreateBlock(int32 id, FVector pos);


	//创建Building Actor
	bool CreateBuilding(int32 id, int32 rotate, FVector pos);

	void GenerateBuilding(Chunk& chunk, GlobalInfo& info);

	void DevelopeDomains(Chunk& chunk, GlobalInfo& info);

	void PlaceBuildings(Chunk& chunk, GlobalInfo& info);

	void PlacePaths(Chunk& chunk, GlobalInfo& info);

	


	bool PlaceOneBuilding(GlobalInfo& info, int32 x, int32 y, int32 buildingIndex, int32 rotate);


	//湿度
	void GenerateHumidity(Chunk& chunk);

	//生物群落分类器
	void GenerateBiome(Chunk& chunk);

	//生成温度
	void GenerateTemperature(Chunk& chunk);

	//生成洞穴
	void GenerateCave(Chunk& chunk, GlobalInfo& info);

	//生成高度
	void GenerateHeight(Chunk& chunk, GlobalInfo& info);

	//生成树
	void GeneratePlant(Chunk& chunk, GlobalInfo& info);
	//生成花
	bool GenerateFlower(Chunk& chunk, GlobalInfo& info, int32 i, int32 j, int32 cystalSize);
	//生成树
	bool GenerateTree(Chunk& chunk, GlobalInfo& info, int32 i, int32 j, int32 cystalSize);
	//生成树叶
	void GenerateLeaves(Chunk& chunk, GlobalInfo& info, int32 x, int32 y, int32 height, int32 radius, int32 targetLeafID);

	
};
