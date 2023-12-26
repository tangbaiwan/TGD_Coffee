// Fill out your copyright notice in the Description page of Project Settings.


#include "Model/InstBlock.h"
#include <queue>
#include "Math/UnrealMathUtility.h"
#include "Tool/NoiseTool.h"
#include "Tool/PathFinder.h"
#include "Async/Async.h"


FCriticalSection AInstBlock::MyMutex;

//检查是否有障碍物（不可走）
bool inBarrier(FVector2D pos);

//计算权值公式
TPair<float, float> weightFormula(FVector2D pos, FVector2D endPos, float cost);

// Sets default values
AInstBlock::AInstBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	InstancedStaticMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMesh"));
	

	FString str = TEXT("/Script/Engine.StaticMesh'/Game/Meshes/Block6.Block6'") ; 
	UStaticMesh* StaticMesh= LoadObject<UStaticMesh>(nullptr, *str);
	InstancedStaticMesh->SetStaticMesh(StaticMesh);

	str = TEXT("/Script/Engine.Material'/Game/Materias/M_Base_01.M_Base_01'");
	UMaterial* DMI = LoadObject<UMaterial>(nullptr, *str);
	//InstancedStaticMesh->CreateDynamicMaterialInstance(0, DMI);
	InstancedStaticMesh->NumCustomDataFloats = 1;
	this->SetRootComponent(InstancedStaticMesh);
	//带一个staticmesh组件
	
}

// Called when the game starts or when spawned
void AInstBlock::BeginPlay()
{
	Super::BeginPlay();
	GameEnd = false;
	roads.Reset();
	domains.Reset();
}

void AInstBlock::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GameEnd = true;
}

// Called every frame
void AInstBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInstBlock::SetCameraLoaction(FVector location)
{
	ChunksCenterPosition = FVector2D(
		floor(location.X / (MaxBlocksWidth * 100) - DisplayCenter),
		floor(location.Y / (MaxBlocksWidth * 100) - DisplayCenter));
}

void AInstBlock::UpdateChunks()
{
	if (Info.GetBlocks2Display().Num() > 0)
		return;
	if (PlayerController)
	{
		PlayerController->GetPlayerViewPoint(PCLocation, PCRotation);
		SetCameraLoaction(PCLocation);
	}
	else
		PlayerController= GetWorld()->GetFirstPlayerController();
	
	//在ChunkSize*ChunkSize的范围内加载Chunk
	for (int i = 0; i < ChunkSize; ++i)
		for (int j = 0; j < ChunkSize; ++j)
		{
			FVector2D chunkPosition = FVector2D(ChunksCenterPosition.X + i,
				ChunksCenterPosition.Y + j);
			uint64 index = NoiseTool::Index(chunkPosition.X, chunkPosition.Y);

			//若未加载该Chunk
			if (!AllChunk.Contains(index)) {
				//创建Chunk
				AllChunk.Emplace(index, Chunk(chunkPosition));
				Chunk& chunk = AllChunk[index];
				//加载chunk信息
				LoadChunk(chunk);
				//加载地形方块ID
				LoadTerrianBlocksID(chunk);
				if (i < ChunkSize - 2&& j < ChunkSize - 2)
				{
					//记录Chunk为已显示
					Chunks2Display.Add(&chunk);

					//生成建筑
					GenerateBuilding(chunk, this->Info);
					//生成植被
					GeneratePlant(chunk, this->Info);
					//显示chunk
					//添加地形方块进显示方块列表

					for (int ii = 0; ii < 16; ++ii)
					{
						for (int jj = 0; jj < 16; ++jj)
						{
							for (int k = chunk.BlocksHeight[ii][jj]; k > chunk.BlocksHeight[ii][jj] - 20; --k)
							{
								FVector pos = FVector(chunk.ChunkPosition.X * 16 + ii, chunk.ChunkPosition.Y * 16 + jj, k);
								int32* blockID = Info.FindBlock(pos);
								if (blockID) {
									Info.AddBlock(pos, *blockID);
								}
							}
						}
					}
						
				}
			}

		}

	DisplayChunk();


}

void AInstBlock::AsyncUpdateChunks(float SleepValue = 1.0f)
{
	//在ChunkSize*ChunkSize的范围内加载Chunk
	PlayerController = GetWorld()->GetFirstPlayerController();
	
	{
		AsyncTask(ENamedThreads::AnyThread, [this, &SleepValue]()
			{
				float Cur_SleepTime = SleepValue;
				while (!GameEnd)
				{
					UpdateChunks();
					FPlatformProcess::Sleep(Cur_SleepTime);
				}
			});
	}

}

bool AInstBlock::Testbool(int32 SleepValue,TArray<int32>& GlobalBlocksID)
{
	/*int32* result = GlobalBlocksID.Find(SleepValue);
	if (!result)
	{
		return false;
	}*/
	return true;
}

void AInstBlock::LoadChunk(Chunk& chunk)
{

	GenerateHeight(chunk, Info);
	//生成洞穴
	GenerateCave(chunk, Info);
	//生成温度
	GenerateTemperature(chunk);
	//生成湿度
	GenerateHumidity(chunk);
	//生成生物群落属性
	GenerateBiome(chunk);
}

void AInstBlock::LoadTerrianBlocksID(Chunk& chunk) {
	//载入地形方块
	for (int i = 0; i < 16; ++i)
		for (int j = 0; j < 16; ++j)
		{
			int k = chunk.BlocksHeight[i][j];
			int Min_k = k - 20;
			for (; k > Min_k; --k)
			{
				int32 targetBlockID = CaculateBlockID(chunk, i, j, k);
				//随机泥土
				if (rand() % 255 >= 250) {
					targetBlockID = 3;
				}
				/*if (dist(gen))
				{
					targetBlockID = 3;
				}*/

				FVector pos = FVector(chunk.ChunkPosition.X * 16 + i, chunk.ChunkPosition.Y * 16 + j, k);
				if (!Info.FindBlock(pos)) {
					Info.AddBlockWithoutDisplay(pos, targetBlockID);
				}
			}

			//垫底方块（非显示）
			for (int k = chunk.BlocksHeight[i][j] - 20; k > chunk.BlocksHeight[i][j] - 23; --k)
			{
				FVector pos = FVector(chunk.ChunkPosition.X * 16 + i, chunk.ChunkPosition.Y * 16 + j, k);
				Info.AddBlockWithoutDisplay(pos, 1);
			}
		}
}


void AInstBlock::GenerateBuildingBlocks() {
	//根据待显示建筑列表，进行多个ABuilding的创建
	auto& buildings2Display = Info.GetBuildings2Display();
	for (auto& tuple : buildings2Display) {
		uint64 posIndex = tuple.Get<0>();
		int32 buildingIndex = tuple.Get<1>();
		int32 rotate = tuple.Get<2>();
		CreateBuilding(buildingIndex, rotate, NoiseTool::UnIndex(posIndex));
	}

	//显示完建筑就可以清理列表了
	buildings2Display.Reset();
}

int32 AInstBlock::CaculateBlockID(Chunk& chunk, int32 i, int32 j, int32 k) {
	if (i < 0 || i >= 16 || j < 0 || j >= 16) { return 0; }
	int32 dk = chunk.BlocksHeight[i][j] - k;
	//地下石头
	if (dk >= 3) { return 2; }
	//地下泥土
	if (dk >= 1) { return 3; }
	//地表方块	
	switch ((BiomeType)chunk.BlocksBiome[i][j])
	{
	case BiomeType::Snow:	return 10; break;
	case BiomeType::Green:	return 1; break;
	case BiomeType::Dry:	return 3; break;
	case BiomeType::Stone:	return 5; break;
	case BiomeType::Desert:	return 4; break;
	default:				return 0; break;
	};
}


void AInstBlock::DisplayChunk() {
	CurCreateChunkList.Reset();
	CurCreatePosList.Reset();
	CurCreateIndexList.Reset();
	//根据待显示方块列表，进行多个ABlock的创建
	auto& blocks2Display = Info.GetBlocks2Display();
	for (auto& itr : blocks2Display) {
		FVector BlockPosition = NoiseTool::UnIndex(itr.Key);
		const int32 dx[6] = { 1,-1,0,0,0,0 };
		const int32 dy[6] = { 0,0,1,-1,0,0 };
		const int32 dz[6] = { 0,0,0,0,-1,1 };
		for (int d = 0; d < 6; ++d)
		{
			FVector pos = FVector(BlockPosition.X + dx[d], BlockPosition.Y + dy[d], BlockPosition.Z + dz[d]);
			auto result = Info.FindBlock(pos);

			bool needCreate = false;
			if (!result) {
				needCreate = true;
			}
			else switch (*result) {
			case 0:
			case 9:
			case 11:
			case 12:
			case 13:
			case 20:
			case 21:
			case 22:
			case 23:
			case 24:
				needCreate = true;
				break;
			}

			if (needCreate&&itr.Value > 0 && itr.Value < MAX_BLOCKS_NUM)
			{
				if (itr.Value == 24)BlockPosition.Z -= 0.5f;
				CurCreatePosList.Add(FTransform(BlockPosition * 100));
				CurCreateIndexList.Add(NoiseTool::Index(BlockPosition.X, BlockPosition.Y, BlockPosition.Z));
				CurCreateChunkList.Emplace(float(itr.Value), BlockPosition);
				break;
			}
		}
	}
	//Get_CreateNums(CurCreateChunkList.Num());


	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			InstancedStaticMesh->AddInstances(this->CurCreatePosList,false);
			for (int32 i = 0; i < this->CurCreateIndexList.Num(); i++)
			{
				InstancedStaticMesh->SetCustomDataValue(AllIndex.Num(), 0, this->CurCreateChunkList[i].Key, true);
				AllIndex.Add(this->CurCreateIndexList[i]);
				GenerateBuildingBlocks();
			}
			this->Info.GetBlocks2Display().Reset();
			
		});

	//for (auto& itr : this->CurCreateChunkList)
	//{
	//	CreateBlock(itr.Key, itr.Value);
	//}
	//this->Info.GetBlocks2Display().Reset();
	
	
	//blocks2Display.Reset();
	
	//显示完方块就可以清理列表了
	
}

void AInstBlock::Get_CreateNums(int32 Cur_CreateNum)
{
	FScopeLock Lock(&MyMutex);
	Cur_CreateNums = Cur_CreateNum;
}

void AInstBlock::Set_CreateNums()
{
	FScopeLock Lock(&MyMutex);
	if (Cur_CreateNums != -1)
	{
		if (Cur_CreateNums == 1)
		{
			auto& blocks2Display = Info.GetBlocks2Display();
			blocks2Display.Reset();
			Cur_CreateNums = -1;

		}
		else
			Cur_CreateNums--;
	}
}


bool AInstBlock::CreateBlock(int32 id, FVector pos)
{
	//InstancedStaticMesh->AddInstance(FTransform(pos * 100));
	InstancedStaticMesh->SetCustomDataValue(AllIndex.Num() - 1, 0, float(id), true);
	GenerateBuildingBlocks();
	
	
	//AsyncTask(ENamedThreads::GameThread, [this, &index, &pos, &id]()
	//	{
	//		InstancedStaticMesh->AddInstance(FTransform(pos * 100));
	//		
	//		GenerateBuildingBlocks();
	//		// 转换为两个 float
	//		/*FloatsFromUInt64 floats;
	//		floats.uint64Value = index;*/
	//		int32 Cur_i = InstancedStaticMesh->GetInstanceCount() - 1;
	//		InstancedStaticMesh->SetCustomDataValue(Cur_i, 0, float(id), true);
	//		Set_CreateNums();
	//		
	//		/*BBlocks->InstancedStaticMesh->SetCustomDataValue(Cur_i, 1, floats.floatParts.floatPart1);
	//		BBlocks->InstancedStaticMesh->SetCustomDataValue(Cur_i, 2, floats.floatParts.floatPart2);
	//		BBlocks->InstancedStaticMesh->SetCustomDataValue(Cur_i, 3, pos.X * 100);
	//		BBlocks->InstancedStaticMesh->SetCustomDataValue(Cur_i, 4, pos.Y * 100);
	//		BBlocks->InstancedStaticMesh->SetCustomDataValue(Cur_i, 5, pos.Z * 100);*/

	//	});

	return true;
}

bool AInstBlock::CreateBuilding(int32 id, int32 rotate, FVector pos) {
	FString str = TEXT("Blueprint'/Game/Blueprints/BP_Building") + FString::FromInt(id) + TEXT(".BP_Building") + FString::FromInt(id) + TEXT("_C'");
	UClass* BPClass = LoadClass<AActor>(nullptr, *str);
	AActor* spawnActor = GetWorld()->SpawnActor<AActor>(BPClass, pos * 100 + FVector(-50, -50, 0), FRotator(0, rotate * 90, 0));

	return spawnActor != nullptr;
}

//TSet<uint64> AInstBlock::domains = {};
//
//TSet<uint64> AInstBlock::roads = {};
//
//FVector2D AInstBlock::startPoint = FVector2D::ZeroVector;
//
//TArray<FVector2D> AInstBlock::buildingPos = {};

void AInstBlock::GenerateBuilding(Chunk& chunk, GlobalInfo& info) {
	if (NoiseTool::rand(chunk.ChunkPosition) < 0.98f)return;

	//domains.Reset();

	DevelopeDomains(chunk, info);
	PlaceBuildings(chunk, info);
	PlacePaths(chunk, info);
}

void AInstBlock::DevelopeDomains(Chunk& chunk, GlobalInfo& info) {

	startPoint = FVector2D(chunk.ChunkPosition.X * 16 + 7, chunk.ChunkPosition.Y * 16 + 7);

	std::priority_queue<
		std::pair<float, FVector2D>,
		std::vector<std::pair<float, FVector2D>>,
		std::greater<std::pair<float, FVector2D>>
	> s;
	s.emplace(0, startPoint);

	int32 count = 0;
	const int32 dx[9] = { 1,-1,0,0,1,-1,1,-1,0 };
	const int32 dy[9] = { 0,0,1,-1,1,-1,-1,1,0 };

	UE_LOG(LogTemp, Warning, TEXT("develop begin"));

	while (!s.empty()) {
		count++;
		float cost = s.top().first;
		FVector2D p = s.top().second;
		s.pop();

		for (int d = 0; d < 9; ++d)
			domains.Emplace(NoiseTool::Index(p.X + dx[d], p.Y + dy[d]));

		//设置最高发展度费用
		if (cost > 7)break;

		int32 centerHeight = info.GetHeight(p.X, p.Y);

		const int32 dx[4] = { 1,-1,0,0 };
		const int32 dy[4] = { 0,0,1,-1 };

		for (int d = 0; d < 4; ++d) {
			int32 x = p.X + dx[d] * 3;
			int32 y = p.Y + dy[d] * 3;

			if (domains.Find(NoiseTool::Index(x, y)))continue;

			int32 height = info.GetHeight(x, y);
			if (height <= SeaLevel || height > 1000) { continue; }

			int32 deltaheight = FMath::Abs(centerHeight - height);

			s.emplace(cost + 0.5f + deltaheight * 0.5f, FVector2D(x, y));
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("devlop %d"), count);
}

void AInstBlock::PlaceBuildings(Chunk& chunk, GlobalInfo& info) {
	//[第几个建筑][长、宽]
	const int32 buildingSize[3][2] = { {10,6},{8,6},{6,6} };
	const int32 dx[4] = { 1,-1,0,0 };
	const int32 dy[4] = { 0,0,1,-1 };

	int32 count = 0;

	std::queue<FVector2D> q;
	q.push(startPoint);

	while (!q.empty()) {
		++count;

		auto pos = q.front();
		q.pop();

		int32 index = NoiseTool::randInt(chunk.ChunkPosition + FVector2D(count, -count) * 107) % 3;
		int32 rotate = NoiseTool::randInt(chunk.ChunkPosition + FVector2D(count, -count) * 17) % 4;

		bool test = PlaceOneBuilding(info, pos.X, pos.Y, index, rotate) || PlaceOneBuilding(info, pos.X, pos.Y, index, (rotate + 1) % 4);
		if (!test) {
			continue;
		}

		int32 offset = NoiseTool::randInt(chunk.ChunkPosition + FVector2D(count, -count) * 67) % 3 + 5;
		int32 offsetX = NoiseTool::randInt(chunk.ChunkPosition + FVector2D(count, count) * 61) % 5 - 2;
		int32 offsetY = NoiseTool::randInt(chunk.ChunkPosition + FVector2D(-count, count) * 117) % 5 - 2;

		for (int i = 0; i < 4; ++i) {
			q.push(FVector2D
			(pos.X + dx[i] * (offset + buildingSize[index][0]) + offsetX,
				pos.Y + dy[i] * (offset + buildingSize[index][1]) + offsetY));
		}
	}
}


bool AInstBlock::PlaceOneBuilding(GlobalInfo& info, int32 x, int32 y, int32 index, int32 rotate) {
	//[第几个建筑][长、宽]
	const int32 buildingSize[3][2] = { {10,6},{8,6},{6,6} };

	int rotateIndex = rotate % 2;
	int updown = buildingSize[index][rotateIndex] / 2;
	int leftright = buildingSize[index][!rotateIndex] / 2;

	float aver = 0;
	for (int i = -updown; i < updown; ++i)
		for (int j = -leftright; j < leftright; ++j)
		{
			//若不在发展域，则无需生成建筑         
			if (!domains.Find(NoiseTool::Index(x + i, y + j))) { return false; }

			//若地面被挖空，则无需生成建筑
			FVector pos3D = FVector(x + i, y + j, info.GetHeight(x + i, y + j));
			int* result = info.FindBlock(pos3D);
			if (result && (*result) == 0) { return false; }

			int32 h = info.GetHeight(x + i, y + j);
			aver += h;
		}

	aver /= (buildingSize[index][0] * buildingSize[index][1]);
	aver = floor(aver + 0.5f);

	//低于海平面，没必要生成房屋
	if (aver <= SeaLevel)return false;

	for (int i = -updown; i < updown; ++i)
		for (int j = -leftright; j < leftright; ++j)
		{
			info.SetHeight(x + i, y + j, aver);
			domains.Remove(NoiseTool::Index(x + i, y + j));
		}

	//地表插入空气，以免生成树木花草
	for (int i = -updown - 1; i < updown + 1; ++i)
		for (int j = -leftright - 1; j < leftright + 1; ++j)
		{
			FVector pos = FVector(x + i, y + j, info.GetHeight(x + i, y + j) + 1);
			info.AddBlock(pos, 0);
		}

	info.AddBuilding(FVector(x, y, aver + 1), index + 1, rotate);
	domains.Emplace(NoiseTool::Index(x - updown, y - leftright));
	buildingPos.Push(FVector2D(x - updown, y - leftright));
	return true;
}

void AInstBlock::PlacePaths(Chunk& chunk, GlobalInfo& info) {
	PathFinder::setConditionInBarrier(inBarrier);
	PathFinder::setWeightFormula(weightFormula);
	int n = buildingPos.Num();
	//两两寻路，进行道路连接
	for (int i = 0; i < n; ++i)
		for (int j = i + 1; j < n; ++j)
		{
			auto path = PathFinder::findPath(buildingPos[i], buildingPos[j]);

			UE_LOG(LogTemp, Warning, TEXT("path roads num = %d"), path.Num());

			for (FVector2D pos : path) {
				roads.Emplace(NoiseTool::Index(pos.X, pos.Y));
				FVector pos3D = FVector(pos.X, pos.Y, info.GetHeight(pos.X, pos.Y));
				int* result = info.FindBlock(pos3D);

				//被挖空则不生成
				if (result && (*result) == 0)
					continue;

				info.AlterBlock(FVector(pos.X, pos.Y, info.GetHeight(pos.X, pos.Y)), 5);
			}
		}
	buildingPos.Reset();
}

bool inBarrier(FVector2D pos) {
	return domains.Contains(NoiseTool::Index(pos.X, pos.Y));
}


TPair<float, float> weightFormula(FVector2D pos, FVector2D endPos, float cost) {
	if (roads.Contains(NoiseTool::Index(pos.X, pos.Y)))
	{
		cost -= 0.5f;
	}

	FVector2D dist = (endPos - pos).GetAbs();
	float predict = (dist.X + dist.Y) * 1.41f - FMath::Max(dist.X, dist.Y) * 0.41f + cost;

	return TPair<float, float>(cost, predict);
}

void AInstBlock::GenerateHumidity(Chunk& chunk)
{
	const int32 times = 3;
	int32 cystalSize = 16;

	NoiseTool::setSeed(201);

	for (int d = 0; d < times; ++d) {

		NoiseTool::prehandleSimplexNoise(chunk.ChunkPosition, cystalSize);

		for (int i = 0; i < MaxBlocksWidth; ++i)
			for (int j = 0; j < MaxBlocksWidth; ++j)
			{
				FVector2D pf = FVector2D((float)i / MaxBlocksWidth / cystalSize, float(j) / MaxBlocksWidth / cystalSize);

				//湿度[0,1]
				float value =
					FMath::Clamp<float>(
						FMath::Abs(NoiseTool::simplexNoise(pf)) + NoiseTool::rand(pf) * 0.05f
						, 0.0f, 1.0f);

				chunk.BlocksHumidity[i][j] += value / times;
			}

		cystalSize *= 2;
	}
}

void AInstBlock::GenerateBiome(Chunk& chunk)
{
	for (int i = 0; i < MaxBlocksWidth; ++i)
		for (int j = 0; j < MaxBlocksWidth; ++j)
		{
			float temperature = chunk.BlocksTemperature[i][j];
			float humidity = chunk.BlocksHumidity[i][j];

			BiomeType targetBiome = BiomeType::None;

			if (temperature < -0.2f)
				targetBiome = BiomeType::Snow;
			else if (temperature < 0.2f && humidity < 0.05f)
				targetBiome = BiomeType::Stone;
			else if (temperature < 0.2f && humidity < 0.1f)
				targetBiome = BiomeType::Dry;
			else if (temperature < 0.2f || humidity > 0.7f)
				targetBiome = BiomeType::Green;
			else
				targetBiome = BiomeType::Desert;

			chunk.BlocksBiome[i][j] = int32(targetBiome);
		}
}

void AInstBlock::GenerateTemperature(Chunk& chunk)
{
	const int32 times = 3;
	int32 cystalSize = 16;

	NoiseTool::setSeed(143);

	for (int d = 0; d < times; ++d) {
		NoiseTool::prehandleSimplexNoise(chunk.ChunkPosition, cystalSize);

		for (int i = 0; i < MaxBlocksWidth; ++i)
			for (int j = 0; j < MaxBlocksWidth; ++j)
			{
				FVector2D pf = FVector2D((float)i / MaxBlocksWidth / cystalSize, float(j) / MaxBlocksWidth / cystalSize);
				//温度[-1,1]
				float value = FMath::Clamp<float>(
					NoiseTool::simplexNoise(pf) + (NoiseTool::rand(pf) - 0.5f) * 0.05f
					, -1.0f, 1.0f);
				chunk.BlocksTemperature[i][j] += value / times;
			}
		cystalSize *= 2;
	}
}

void AInstBlock::GenerateCave(Chunk& chunk, GlobalInfo& info)
{
	//晶格大小
	int32 m = 2;

	NoiseTool::prehandleSimplexNoise(FVector(chunk.ChunkPosition.X, chunk.ChunkPosition.Y, 0), m);

	for (int i = 0; i < MaxBlocksWidth; ++i)
		for (int j = 0; j < MaxBlocksWidth; ++j)
		{
			int32 mh = info.GetHeight(chunk.ChunkPosition.X * 16 + i, chunk.ChunkPosition.Y * 16 + j);
			for (int k = chunk.BlocksHeight[i][j]; k >= chunk.BlocksHeight[i][j] - 10; --k)
			{
				FVector pf = FVector(
					float(i) / MaxBlocksWidth / m,
					float(j) / MaxBlocksWidth / m,
					float(k) / MaxBlocksWidth / m
				);

				//若高于一定阈值，挖空
				if (NoiseTool::simplexNoise(pf) + (chunk.BlocksHeight[i][j] - k) / 10.0f * 0.3f > 0.5f) {
					uint64 index = NoiseTool::Index(
						chunk.ChunkPosition.X * 16 + i,
						chunk.ChunkPosition.Y * 16 + j,
						k);

					info.AddBlock(FVector(
						chunk.ChunkPosition.X * 16 + i,
						chunk.ChunkPosition.Y * 16 + j,
						k)
						, 0);
				};

				mh = FMath::Max(mh, k);
			}

			info.SetHeight(chunk.ChunkPosition.X * 16 + i, chunk.ChunkPosition.Y * 16 + j, mh);
		}
}

void AInstBlock::GenerateHeight(Chunk& chunk, GlobalInfo& info)
{
	//晶格大小
	int32 cystalSize[3] = { 4,16,64 };
	//权重
	float weight[3] = { 0.12f,0.22f,0.66f };
	//波幅
	float maxHeigh[3] = { 100.0f,100.0f,100.0f };

	for (int d = 0; d < 3; ++d) {

		//预处理simplex噪声
		NoiseTool::prehandleSimplexNoise(chunk.ChunkPosition, cystalSize[d]);

		for (int i = 0; i < MaxBlocksWidth; ++i)
			for (int j = 0; j < MaxBlocksWidth; ++j)
			{
				FVector2D pf = FVector2D(float(i) / MaxBlocksWidth / cystalSize[d], float(j) / MaxBlocksWidth / cystalSize[d]);
				chunk.BlocksHeight[i][j] += (NoiseTool::simplexNoise(pf) * maxHeigh[d] + maxHeigh[d]) * weight[d];
			}
	}

	//更新全局信息中的高度
	info.SetChunkHeight(chunk.ChunkPosition.X, chunk.ChunkPosition.Y, chunk.BlocksHeight);
}

void AInstBlock::GeneratePlant(Chunk& chunk, GlobalInfo& info)
{
	const int32 cystalSize = 16;
	int32 seedOffset = NoiseTool::hash21(chunk.ChunkPosition);

	NoiseTool::prehandleSimplexNoise(chunk.ChunkPosition, cystalSize);
	NoiseTool::setSeed(1317 + seedOffset);

	for (int i = 0; i < MaxBlocksWidth; ++i)
		for (int j = 0; j < MaxBlocksWidth; ++j)
		{
			//不允许在空地、石头、沙漠上产生植被
			if (chunk.BlocksBiome[i][j] == static_cast<int32>(BiomeType::None) ||
				chunk.BlocksBiome[i][j] == static_cast<int32>(BiomeType::Stone) ||
				chunk.BlocksBiome[i][j] == static_cast<int32>(BiomeType::Desert))
			{
				continue;
			}

			int32* result;
			//查询地板方块ID
			result = info.FindBlock(FVector(
				chunk.ChunkPosition.X * 16 + i,
				chunk.ChunkPosition.Y * 16 + j,
				chunk.BlocksHeight[i][j])
			);
			//被挖空（方块ID为0）或者方块ID为9（方块为水）就无法生成
			if (result && (*result == 0 || *result == 9)) {
				continue;
			}

			//查询地板上一格的方块ID
			result = info.FindBlock(FVector(
				chunk.ChunkPosition.X * 16 + i,
				chunk.ChunkPosition.Y * 16 + j,
				chunk.BlocksHeight[i][j] + 1));
			//如果存在方块，则无法生成
			if (result) {
				continue;
			}

			//生成树
			if (GenerateTree(chunk, info, i, j, cystalSize))continue;

			//生成草
			if (GenerateFlower(chunk, info, i, j, cystalSize))continue;
		}
}

bool AInstBlock::GenerateFlower(Chunk& chunk, GlobalInfo& info, int32 i, int32 j, int32 cystalSize)
{
	//如果地板方块在水平面以下，则不允许生成花朵
	if (chunk.BlocksHeight[i][j] <= SeaLevel)
		return false;

	//获得柱块的温度值、湿度值
	float temperature = chunk.BlocksTemperature[i][j];
	float humidity = chunk.BlocksHumidity[i][j];

	FVector2D pf = FVector2D(float(i) / MaxBlocksWidth / cystalSize, float(j) / MaxBlocksWidth / cystalSize);
	//计算概率值
	float possible = NoiseTool::rand(pf) - FMath::Abs(temperature + 0.1f) * 0.4f + humidity * 0.4f;

	int32 targetID = 0;
	//若满足生成花的概率
	if (possible > 1.0f) {
		//方块ID：20=黄花、21=蓝花、22=蘑菇、23=蘑菇
		targetID = 20 + NoiseTool::randInt(FVector2D(i + j * 21, j - i ^ 2)) % 4;
	}
	//若满足生成草的概率
	else if (possible > 0.85f) {
		//方块ID：11=绿草 12=黄草，13=白草
		if (temperature > 0.3f) { targetID = 12; }
		else if (temperature > -0.3f) { targetID = 11; }
		else { targetID = 13; }
	}
	if (targetID == 0)
		return false;

	//添加目标方块（花或草）    
	info.AddBlock(FVector(
		chunk.ChunkPosition.X * 16 + i,
		chunk.ChunkPosition.Y * 16 + j,
		chunk.BlocksHeight[i][j] + 1), targetID);

	return true;
}

bool AInstBlock::GenerateTree(Chunk& chunk, GlobalInfo& info, int32 i, int32 j, int32 cystalSize)
{
	//如果地板方块在水平面下一格的以下，则不允许生成花朵
	if (chunk.BlocksHeight[i][j] <= SeaLevel - 1)
		return false;

	FVector2D pf = FVector2D(float(i) / MaxBlocksWidth / cystalSize, float(j) / MaxBlocksWidth / cystalSize);
	float temperature = chunk.BlocksTemperature[i][j];
	float humidity = chunk.BlocksHumidity[i][j];
	float possible = (NoiseTool::simplexNoise(pf) + 1.0f) / 2.0f * 0.15f - FMath::Abs(temperature + 0.1f) * 0.10f + humidity * 0.15f + NoiseTool::rand(-pf) * 0.9f;

	//若满足概率
	if (possible < 0.985f)
		return false;
	//树干高度
	int32 treeHeight = (NoiseTool::randInt(FVector2D::UnitVector - pf) % 3) + 4;

	//根据温度选择树类型
	temperature += (NoiseTool::rand(FVector2D::UnitVector + pf) - 0.5f) * 0.2f;

	int32 targetWoodID;
	int32 targetLeafID;
	//方块ID：14=温带木 15=热带木，16=寒带木，17=温带树叶 18=寒带树叶，19=热带树叶
	if (temperature > 0.3f) { targetWoodID = 15; targetLeafID = 19; }
	else if (temperature > -0.3f) { targetWoodID = 14; targetLeafID = 17; }
	else { targetWoodID = 16; targetLeafID = 18; }

	for (int k = 0; k < treeHeight; ++k) {
		//生成目标方块（树干）    
		info.AddBlock(FVector(
			chunk.ChunkPosition.X * 16 + i,
			chunk.ChunkPosition.Y * 16 + j,
			chunk.BlocksHeight[i][j] + 1 + k), targetWoodID);
	}

	int32 t1 = NoiseTool::rand(17 * pf) * 4 + 1.5f + int32(treeHeight >= 5);
	int32 t2 = NoiseTool::rand(11 * pf) * 4 + 1.5f + int32(treeHeight >= 5);
	int32 leafHeight = treeHeight + 1 + t1 % 3;
	int32 initLeafHeight = 2 + t2 % 2;
	for (int k = leafHeight - 1; k >= initLeafHeight; --k) {
		//贝塞尔曲线计算树叶半径
		float leafRadius =
			NoiseTool::bezier(
				FVector2D(0, 0)
				, FVector2D(0.33f, t1)
				, FVector2D(0.66f, t2)
				, FVector2D(1, 0)
				, float(k - initLeafHeight) / (leafHeight - 1 - initLeafHeight)).Y;

		//生成树叶
		GenerateLeaves(chunk, info, i, j, chunk.BlocksHeight[i][j] + 1 + k, leafRadius, targetLeafID);
	}

	return true;
}

void AInstBlock::GenerateLeaves(Chunk& chunk, GlobalInfo& info, int32 x, int32 y, int32 height, int32 radius, int32 targetLeafID)
{
	radius = FMath::Clamp(radius, 0, 3);

	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j) {
			if (!leavesTemplate[radius][i][j])
				continue;

			int32 dx = x + i - 2;
			int32 dy = y + j - 2;
			int32 gx = dx + chunk.ChunkPosition.X * 16;
			int32 gy = dy + chunk.ChunkPosition.Y * 16;

			if (info.FindBlock(FVector(gx, gy, height)))
				continue;

			info.AddBlock(FVector(gx, gy, height), targetLeafID);

			//树顶雪
			if (chunk.BlocksBiome[x][y] == 1 && NoiseTool::rand(FVector2D(gx + height * 11, gy * 17 + radius * 23)) > 0.13f) {
				info.AddBlock(FVector(gx, gy, height + 1), 24);
			}
		}
}
