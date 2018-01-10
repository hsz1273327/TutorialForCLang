#include <stdio.h>
#include <wchar.h>
#include <locale.h>
/* 定义 */
typedef struct {
    const wchar_t *name;
    const wchar_t *species;
    int age;
} Aquatic;


/* 打印目录项 */
void catalog(Aquatic *aquatic){
    wprintf(L"%ls 是一种的 %ls.这只%ls已经%i岁了\n",
        aquatic->name,aquatic->species,aquatic->name,aquatic->age);
    
}

/* 打印贴在水缸上的标签 */
void label(Aquatic *aquatic){
    wprintf(L"名字: %ls\n种类: %ls\n%i 岁.\n",
       aquatic->name,aquatic->species,aquatic->age);
}

int main(){
    setlocale(LC_ALL, "zh_CN.UTF-8");   
    Aquatic mantis_shrimp = {L"皮皮虾",L"节肢动物",2};
    catalog(&mantis_shrimp);
    label(&mantis_shrimp);
    return 0;
}