#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <vector>

#include "Utils\CPatch.h"

#include "dllmain.h"

const LPCSTR pluginConfigPath = ".\\KoTR_ResourceLoader.ini";
//const LPCSTR gameConfigPath   = ".\\truck.ini";

using namespace std;

char file_path[256];
char file_name[256];
char section[32];

char common_b3d_path[128];
char cabines_b3d_path[128];
char trucks_b3d_path[128];

char env_path[128];
char menv_path[128];
char mtb_path[128];
char mtr_path[128];

//char base_install[256];
char* base_install;
char path[256];

int file_index = 1;

bool consoleEnabled;
bool loggingEnabled;
bool ignore_texture_settings;

DWORD* common_res; //указатель на common.res
vector<string> sound_files_list; //все звуковые файлы
vector<string> sound_names_list;

vector<string> module_files_list; //все модули
vector<string> module_names_list;

vector<string> res_files_list; //все res-файлы
vector<string> res_names_list;

void WriteDebugLog(const char* text) {
	FILE* file = fopen(".\\KoTR_ResourceLoader.log", "a");
	if (file != NULL) {
		fputs(text, file);
		fputs("\n", file);
		fclose(file);
	}
}

void ReadConfigs() {
	char buffer[32];

	GetPrivateProfileStringA("common", "DisplayConsole", "off", buffer, 32, pluginConfigPath);
	if (!strcmp(buffer, "on")) {
		consoleEnabled = true;
	}

	GetPrivateProfileStringA("common", "WriteLog", "off", buffer, 32, pluginConfigPath);
	if (!strcmp(buffer, "on")) {
		loggingEnabled = true;
		remove(".\\KoTR_ResourceLoader.log");
	}

	GetPrivateProfileStringA("common", "ignore_texture_settings", "on", buffer, 32, pluginConfigPath);
	if (!strcmp(buffer, "on")) {
		ignore_texture_settings = true;
	}

	GetPrivateProfileStringA("common", "common_path", "COMMON\\", common_b3d_path, 128, pluginConfigPath);
	GetPrivateProfileStringA("common", "cabines_path", "COMMON\\", cabines_b3d_path, 128, pluginConfigPath);
	GetPrivateProfileStringA("common", "trucks_path", "COMMON\\", trucks_b3d_path, 128, pluginConfigPath);

	GetPrivateProfileStringA("common", "env_path", "ENV\\", env_path, 128, pluginConfigPath);
	GetPrivateProfileStringA("common", "menv_path", "MENV\\", menv_path, 128, pluginConfigPath);
	GetPrivateProfileStringA("common", "mtb_path", "MTB\\", mtb_path, 128, pluginConfigPath);
	GetPrivateProfileStringA("common", "mtr_path", "MTR\\", mtr_path, 128, pluginConfigPath);

	//GetPrivateProfileStringA("INSTALL", "base", ".", base_install, 256, gameConfigPath);
}

//05DC1B0
char* __fastcall GetFullPath(DWORD* _this, DWORD EDX, char* subPath) {
	//std::cout << "GetFullPath\n";
	//std::cout << subPath << std::endl;

	int* TheGamePtr = *(int**)0x696CC0;
	//здесь первый раз используется base_install, так что и значение присваивается тут
	base_install = *(char**)((char*)TheGamePtr + 0x38 + 0x250);
	
	//int* base_off = (int*)((char*)TheGamePtr + 0x38 + 0x250);
	//strcpy(base_install, (char*)(*base_off));
	
	//std::cout << base_install << std::endl;

	memset(path, 0, 256);

	if (strcmp(base_install, ".\\")) {
		strcpy(path, base_install);
	}

	if (!strcmp(subPath, ".\\ENV")) {
		strcat(path, env_path);
	}
	else if (!strcmp(subPath, ".\\MENV")) {
		strcat(path, menv_path);
	}
	else if (!strcmp(subPath, ".\\MTR")) {
		strcat(path, mtr_path);
	}
	else {
		strcat(path, subPath);
	}

	return path;
}

char* __fastcall GetFullPathToFile(DWORD* _this, DWORD EDX, const char* subFolder, char* fileName) {
	//std::cout << "GetFullPathToFile\n";

	if (!strcmp(base_install, ".\\"))
		memset(path, '\0', 128);
	else
		strcpy(path, base_install);

	//cout << "subFolder: " << subFolder << endl;
	//cout << "fileName: " << fileName << endl;
	//cout << "path: " << path << endl;

	//cout << "strcmp(subFolder, ENV)" << strcmp(subFolder, "ENV") << endl;
	//cout << "strcmp(subFolder, ENV\\)" << strcmp(subFolder, "ENV\\") << endl;

	//cout << "env_path: " << env_path << endl;

	if (!strcmp(subFolder, "ENV\\")) {        // "ENV\\"
		strcpy((char*)subFolder, env_path);
	}
	else if (!strcmp(subFolder, "MTR\\")) { // "MTR\\"
		strcpy((char*)subFolder, mtr_path);
	}

	if (strcmp(subFolder, ".\\"))
		strcat(path, subFolder);
	strcat(path, fileName);

	return path;
}

DWORD* LoadAndInitGameModule(char* modulePath, char* b3dName, char* resName) {
	//DWORD* TheGame_addr = (DWORD*)(*(DWORD*)0x696CC0);
	typedef DWORD* (__thiscall* LoadAndInitGameModule)(int* _this, char* modulePath, char* b3dName, char* resName);
	int* TheGamePtr = *(int**)0x696CC0;
	return LoadAndInitGameModule(0x5DAE10)(TheGamePtr, modulePath, b3dName, resName);
}

DWORD* __cdecl LoadGameModule(const char* path, const char* name) {
	char modulePath[255];

	char b3dName[128];
	char resName[128];

	DWORD MemState = *(DWORD*)0x6CED90; //видимо, параметр качества текстур
	DWORD* result; //указатель на загруженный игровой модуль

	//strcpy(m_sPath, path);

	strcpy(b3dName, name);
	strcat(b3dName, ".b3d");

	strcpy(resName, name);

	//если включён параметр "игнорирования" текстурных настроек,
	//то загружать текстуры самого высокого качества (см. case 4)
	if (ignore_texture_settings) {
		MemState = 4;
	}

	//это определение названия архива с текстурами
	//*.res - самое высокое качество, *2.res - самое низкое (*3.res для trucks)
	switch (MemState) {
	case 0:
		//если trucks, то *3.res
		if (!strcmp(resName, "trucks")) {
			strcat(resName, "3.res");
		}
		else {
			strcat(resName, "2.res");
		}
		break;
	case 1:
		//если trucks, то *2.res
		if (!strcmp(resName, "trucks")) {
			strcat(resName, "2.res");
		}
		else {
			strcat(resName, "1.res");
		}
		break;
	case 2:
		strcat(resName, "1.res");
		break;
	case 3:
		//если trucks, то *1.res
		if (!strcmp(resName, "trucks")) {
			strcat(resName, "1.res");
		}
		else {
			strcat(resName, ".res");
		}
		break;
	case 4:
		strcat(resName, ".res");
		break;
	default:
		strcat(resName, ".res");
		break;
	}

	strcpy(modulePath, GetFullPath(0, 0, (char*)path));

	if (loggingEnabled) {
		char buffer[255];
		sprintf(buffer, "%d. Loading %s, %s ->\nmodulePath=%s, b3dName=%s, resName=%s\n", file_index, path, name, modulePath, b3dName, resName);
		WriteDebugLog(buffer);
	}

	cout << file_index << ". Loading """ << path << """, """ << name << """" << endl;
	file_index++;

	result = LoadAndInitGameModule(modulePath, b3dName, resName);

	return result;
}

void LoadModules() {
	int load_count = module_files_list.size();

	for (int i = 0; i < load_count; i++) {
		//std::cout << "FILE=[" << module_files_list[i] << "] name [" << module_names_list[i] << "]\n";
		//system("pause");

		LoadGameModule(module_files_list[i].c_str(), module_names_list[i].c_str());
	}

	module_files_list.clear();
	module_names_list.clear();
}

void LoadResourcesList(string path, string fileName) {
	//std::cout << "LoadResourcesList" << std::endl;

	string fullPath = path + "\\";
	string sFilePath = fullPath + fileName;
	const char* filePath = sFilePath.c_str();

	int iter = 1;

	while (true) {
		sprintf(section, "MODULE_%d", iter);
		GetPrivateProfileStringA(section, "path", "", file_path, 256, filePath);
		GetPrivateProfileStringA(section, "name", "none", file_name, 256, filePath);

		iter++;

		if (!strcmp(file_name, "none")) {
			break;
		}

		module_files_list.push_back(fullPath + file_path);
		module_names_list.push_back(file_name);

		//module_files_list.push_back(file_path);
		//module_names_list.push_back(file_name);

		//LoadGameModule(file_path, file_name);
	}

	//заполнение списка файлов ресурсов
	iter = 1;

	while (true) {
		sprintf(section, "RESOURCE_%d", iter);
		GetPrivateProfileStringA(section, "path", "", file_path, 256, filePath);
		GetPrivateProfileStringA(section, "name", "none", file_name, 256, filePath);

		iter++;

		if (!strcmp(file_name, "none")) {
			break;
		}

		res_files_list.push_back(fullPath + file_path);
		res_names_list.push_back(file_name);


		//res_files_list.push_back(file_path);
		//res_names_list.push_back(file_name);
	}

	//заполнение списка звуковых файлов
	iter = 1;

	while (true) {
		sprintf(section, "SOUND_%d", iter);

		GetPrivateProfileStringA(section, "path", "none", file_path, 256, filePath);
		GetPrivateProfileStringA(section, "name", "none", file_name, 256, filePath);

		iter++;

		if (!strcmp(file_path, "none") || !strcmp(file_name, "none")) {
			break;
		}

		//std::cout << "file_path=" << file_path << " file_name=" << file_name << std::endl;
		
		sound_files_list.push_back(fullPath + file_path);
		sound_names_list.push_back(file_name);

		//char snd_full_path[256];
		//strcpy(snd_full_path, base_install);
		//strcat(snd_full_path, file_path);

		//sound_files_list.push_back(snd_full_path);
		//sound_names_list.push_back(file_name);
	}
}

void SearchLoadLists(string basePath) {
	//std::cout << "PATH=" << basePath.c_str() << std::endl;

	string searchPath = base_install + basePath + "\\*.*";

	WIN32_FIND_DATA findData;
	HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			//std::cout << "[" << findData.cFileName << "]" << std::endl;
			//std::cout << strlen(findData.cFileName) << std::endl;

			//чтобы отсеять "." и ".."
			if (strlen(findData.cFileName) > 2) {
				if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					//std::cout << "DIR: " << findData.cFileName << std::endl;
					string nextDir = basePath + "\\" + findData.cFileName;
					SearchLoadLists(nextDir);
				}
				else {
					string fileName = findData.cFileName;
					if (fileName.length() > 5 && !fileName.compare(fileName.length() - 4, 4, ".lst")){
						//std::cout << "LIST: " << findData.cFileName << std::endl;
						LoadResourcesList(basePath, findData.cFileName);
					}
				}
			}

		} while (FindNextFile(hFind, &findData));
	}
	//else {
	//	std::cout << "SearchLoadLists::hFind == INVALID_HANDLE_VALUE\n";
	//}
	
	FindClose(hFind);
}


DWORD* LoadSoundFileFromPath(char* path) {
	typedef DWORD* (*LoadSoundFileFromPath)(char* path);
	return LoadSoundFileFromPath(0x5E97A0)(path);
}

DWORD* sub_5FCF40(const char* path, int id, int data) {
	typedef DWORD* (*sub_5FCF40)(const char* path, int id, int data);
	return sub_5FCF40(0x5FCF40)(path, id, data);
}

DWORD* sub_5FD020(FILE* file, int file_ID, int file_size, int a4) {
	typedef DWORD* (*sub_5FD020)(FILE* file, int file_ID, int file_size, int a4);
	return sub_5FD020(0x5FD020)(file, file_ID, file_size, a4);
}

int InitSoundFilesDataArray(int count) {
	typedef int(*InitSoundFilesDataArray)(int count);
	return InitSoundFilesDataArray(0x604CE0)(count);
}

int DeinitSoundFilesDataArray() {
	typedef int(*DeinitSoundFilesDataArray)();
	return DeinitSoundFilesDataArray(0x604D80)();
}

DWORD* PrepareRESSound(DWORD* _this, const char* name, int fileID, int a4) {
	typedef DWORD* (__thiscall* PrepareRESSound)(DWORD* _this, const char* name, int fileID, int a4);
	return PrepareRESSound(0x5FD330)(_this, name, fileID, a4);
}

void RESSound__destructor(DWORD* _this) {
	typedef void(__thiscall* RESSound__destructor)(DWORD* _this);
	return RESSound__destructor(0x4DDA90)(_this);
}

void AddStaticNode(DWORD* _this, const char* name, void* cb) {
	typedef void(__thiscall* AddStaticNode)(DWORD* _this, const char* name, void* cb);
	return AddStaticNode(0x5D24B0)(_this, name, cb);
}

int SearchResourceSND(char* a1) {
	typedef int(*GetResource)(char* a1);
	int ret = GetResource(0x5EF380)(a1);
	return ret;
}

void* _malloc_emb(size_t _size) {
	typedef void* (*_malloc_emb)(size_t _size);
	return _malloc_emb(0x44BFB0)(_size);
}

void IncreaseRESSoundArray(int add_count) {
	DWORD* sounds_count = (DWORD*)((char*)common_res + 0x70); //кол-во элементов исходного массива
	DWORD* sounds_array = (DWORD*)((char*)common_res + 0x74); //указатель на массив (GameModule->m_pSounds)
	//cout << "common.res OLD snd_cnt: " << dec << *sounds_count << endl;
	//cout << "common.res OLD snd ptr: " << hex << *sounds_array << endl;

	//копирование массивов тут
	int snd_array_size_old = *sounds_count * 4;
	int snd_array_size_new = (*sounds_count + add_count) * 4;

	DWORD* snd_data = (DWORD*)_malloc_emb(snd_array_size_new); //указатель на новый массив

	//memset(snd_data, 0, snd_array_size_new); //заполнение нового массива нулями
	memcpy(snd_data, (void*)(int*)common_res[29], snd_array_size_old); //копирование старых данных в новый массив

	//нельзя удалять объекты, они ж всё равно в новый массив переходят!
	//а вот заполнить нулями старый массив можно, но нужно ли?
	memset((void*)(int*)common_res[29], 0, snd_array_size_old); //заполнение старого массива нулями (типа очистка массива)

	//освобождение памяти старого массива
	if (sounds_array)
		free(sounds_array);

	sounds_array = (DWORD*)((char*)common_res + 0x74);
	*sounds_array = (DWORD)snd_data; //замена указателя в common.res со старого на новый массив

	sounds_array = (DWORD*)((char*)common_res + 0x74);
	//cout << "common.res NEW snd ptr: " << hex << *sounds_array << endl;

	*sounds_count += add_count;
	//cout << "common.res NEW snd_cnt: " << dec << *sounds_count << endl;
}

void addRESSound(const char* snd_name, int snd_index, int file_ID) {
	DWORD* snd_data_addr = (DWORD*)_malloc_emb(0x20u); //(DWORD*)malloc(0x20u);
	DWORD* res_snd = PrepareRESSound(snd_data_addr, snd_name, file_ID, 0); //05FD330
	//включение параметра зацикливания
	res_snd[2] = 1;

	//добавление звука в GameModule->m_pSounds[snd_index]
	*(DWORD*)(*((DWORD*)common_res + 29) + 4 * snd_index) = (DWORD)res_snd;

	AddStaticNode((DWORD*)0x71E840, snd_name, res_snd);

	//cout << snd_name << " loaded\n";
}

void LoadWAV(const char* wav_name, int sound_file_id) {
	strcpy((char*)0x7217DC, ".\\"); //символы отсюда добавляются к пути файла, обычно там "//COMMON", поэтому перезапись
	DWORD* wav = sub_5FCF40(wav_name, sound_file_id, 0);

	AddStaticNode((DWORD*)0x71E840, wav_name, wav);
}

void LoadSounds() {
	int add_count = sound_files_list.size();

	if (!add_count) {
		return;
	}

	//cout << "Sounds:\n";

	int sound_files_count = *(int*)0x723B50;
	int sound_res_count = *(int*)((char*)common_res + 0x70);

	IncreaseRESSoundArray(add_count);

	sound_files_count -= add_count; //т.к. кол-во уже изменено в перехваченном InitSoundArray

	for (int i = 0; i < add_count; i++) {
		//cout << i + 1 << ". " << sound_files_list[i].c_str() << endl;
		//cout << sound_files_list[i].c_str() << " " << sound_names_list[i].c_str() << endl;
		LoadWAV(sound_files_list[i].c_str(), sound_files_count + i);
		addRESSound(sound_names_list[i].c_str(), sound_res_count + i, sound_files_count + i);

		if (loggingEnabled){
			char buffer[255];
			sprintf(buffer, "%d. Loading SND\n%s -> %s\n", i, sound_files_list[i].c_str(), sound_names_list[i].c_str());
			WriteDebugLog(buffer);
		}
	}

	cout << "Loaded " << add_count << " additional sounds.\n";

	sound_files_list.clear();
	sound_names_list.clear();
}

DWORD* __cdecl LoadRES(char* path, char* name) {
	typedef DWORD* (__cdecl* sub_510970)(char* path, char* name);
	return sub_510970(0x510970)(path, name);
}

void LoadResources() {
	int add_count = res_files_list.size();

	if (!add_count) {
		return;
	}

	cout << "Resources:\n";

	for (int i = 0; i < add_count; i++) {
		cout << i + 1 << ". " << res_files_list[i].c_str() << ", " << res_names_list[i].c_str() << endl;
		LoadRES((char*)res_files_list[i].c_str(), (char*)res_names_list[i].c_str());
		
		if (loggingEnabled) {
			char buffer[255];
			sprintf(buffer, "%d. Loading RES\n%s -> %s\n", file_index, res_files_list[i].c_str(), res_names_list[i].c_str());
			WriteDebugLog(buffer);
		}
	}

	res_files_list.clear();
	res_names_list.clear();
}

DWORD* __cdecl LoadGameResources(const char* path, const char* name) {
	//std::cout << "LoadGameResources\n";

	DWORD* result;

	//в стандартной игре сначала загружается модуль common,
	//затем cabines и следом trucks

	if (!strcmp(name, "common")) {
		if (file_index != 1) { //сброс вывода на экран,
			file_index = 1;    //если загрузка начата заново
			cout << "\n\n\n";
		}

		//Поиск и загрузка списков из папки MODS
		SearchLoadLists("MODS");

		//LoadResourcesList(pluginConfigPath); //подгрузка модулей из ini

		result = LoadGameModule(common_b3d_path, name);
		common_res = result;

	}
	else if (!strcmp(name, "cabines")) {
		result = LoadGameModule(cabines_b3d_path, name);
	}
	else if (!strcmp(name, "trucks")) {
		result = LoadGameModule(trucks_b3d_path, name);

		//system("pause");

		LoadModules();
		LoadResources();
		LoadSounds();
	}
	else {
		result = LoadGameModule(path, name);
	}

	return result;
}

int InitSoundFilesArray(int count) {
	int result;
	//int i;

	count += sound_files_list.size();
	
	//cout << "InitSoundFilesArray: " << count << endl;

	result = *(DWORD*)0x723B48;
	if (result) {
		*(DWORD*)0x723B50 = count;
		*(DWORD*)0x723B54 = (DWORD)(DWORD*)_malloc_emb(4 * count);
		//for ( i = 0; i < count; *((DWORD *)0x723B54 + i - 1) = 0 )
		//	++i;
		result = InitSoundFilesDataArray(count);
	}
	return result;
}

int __fastcall GetModulePath(DWORD* _this, DWORD EDX, const char* moduleName, char* modulePath, char* b3dName, char* resName) {
	//std::cout << "GetModulePath\n";
	
	const char* v5;
	int v6;
	//unsigned int v7;
	int result;
	//const char* v9;

	strcpy(modulePath, base_install);

	if (*(DWORD*)((*(DWORD*)0x6CEC8C) + 0x524))
	{
		//cout << "multiplayer\n";
		v6 = *(DWORD*)((*(DWORD*)0x6CECB8) + 0x180);
		if (v6) {
			if (v6 != 1) {
				memcpy(&modulePath[strlen(modulePath)], mtb_path, strlen(mtb_path) + 1);
				//cout << "mtb\n";
			}
			else {
				memcpy(&modulePath[strlen(modulePath)], mtr_path, strlen(mtr_path) + 1);
				//cout << "mtr\n";
			}
		}
		else {
			memcpy(&modulePath[strlen(modulePath)], menv_path, strlen(menv_path) + 1);
			//cout << "menv\n";
		}
	}
	else {
		memcpy(&modulePath[strlen(modulePath)], env_path, strlen(env_path) + 1);
		//cout << "env\n";
	}
	//v7 = strlen(v5) + 1;
	//memcpy(&modulePath[strlen(modulePath)], v5, v7);

	strcpy(b3dName, moduleName);
	strcat(b3dName, ".b3d");
	strcpy(resName, moduleName);

	//cout << "TheGame=" << hex << _this << endl;
	//cout << "moduleName=" << moduleName << endl;
	//cout << "modulePath=" << modulePath << endl;
	//cout << "b3dName=" << b3dName << endl;
	//cout << "resName=" << resName << endl;

	//system("pause");

	//cout << modulePath << endl;

	DWORD MemState = *(DWORD*)0x6CED90;

	if (ignore_texture_settings) {
		MemState = 4;
	}

	result = MemState;
	switch (MemState) {
	case 1:
		strcat(resName, "1.res");
	case 2:
		strcat(resName, "1.res");
		break;
	case 3:
		strcat(resName, ".res");
		break;
	case 4:
		strcat(resName, ".res");
		break;
	default:
		if (MemState)
			return result;

		strcat(resName, "2.res");
		break;
	}

	result = 0;

	return result;
}

void DisplayConsole() {
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
}

void AttachHooks() {
	CPatch::RedirectJump(0x510720, &LoadGameResources);
	CPatch::RedirectJump(0x5FCE60, &InitSoundFilesArray);
	CPatch::RedirectJump(0x512510, &GetModulePath);
	CPatch::RedirectJump(0x5DC1B0, &GetFullPath);
	CPatch::RedirectJump(0x5DC250, &GetFullPathToFile);

	cout << "ResourceLoader v1.07 (10.10.2026) started." << endl;
	cout << "vk.com/rnr_mods" << endl;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{

	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			ReadConfigs();

			if (consoleEnabled) {
				DisplayConsole();
			}

			AttachHooks();

			break;
		}
		case DLL_PROCESS_DETACH:
		{
			break;
		}
		case DLL_THREAD_ATTACH:
		{
			break;
		}
		case DLL_THREAD_DETACH:
		{
			break;
		}
	}

	return TRUE;
}
