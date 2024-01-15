/***************************************************************
 *                                                             *
 * Judul Program     : Tekkom Fighters                         *
 * Kelompok          : 04                                      *
 * Nama Pembuat      : 1. Evandita Wiratama Putra (2206059572) *
 *                     2. Yoel Dwi Miryano (2206059534)        *
 * Release Date      : 28/05/2023                              *
 * Versi Program     : 1.0.0                                   *
 *                                                             *
****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <omp.h>

struct senjata{
    float base_attack;
    float crit_rate;
    int rarity;
    char name[100];
} Pedang, Tongkat, Busur, Senapan, Pisau, Sendal_Jepit, Kursi_Lipat, Peluit_Biasa, Peluit_Ultrasonik, Rocket_Launcher;

struct armor{
    float health;
    float dodge;
    int rarity;
    char name[100];
} Baju_Kuliah, Seragam_Sekolah, Baju_Tidur, Hoodie, Kemeja, Baju_Tradisional, Kaos_Dalam, Jaket_Departemen, Jas_Hitam, Jaket_Kuning;

struct skill{
    //damage
    float dmg_multiplier;       
    //heal
    float hp_regenerate;
    //Buffer      
    float crit_rate_inc;
    float crit_damage_inc;
    float attack_inc;
    float health_inc;
    float dodge_inc;
    int rarity;
    char name[100];
} Pukulan_Biasa, Tendangan_Biasa, Ujaran_Kebencian, Pernafasan_Api, Bola_Air, Fitnah, Tamparan_Fakta, Perisai_Kebenaran, Hujan_1001_Meteor, Orang_Dalam;

struct inventory{
    struct senjata inv_senjata;
    struct armor inv_armor;
    struct skill inv_skill[3];
};

struct character{
    struct inventory inv;
    float base_attack;
    float base_health;
    float base_crit_damage;

    int level;
    char name[100];
};

struct enemy
{
    float ebase_attack;
    float ebase_health;
    float ebase_crit;
    float flee_chance;
    float player_flee;
    float ebase_critrate;

    int elevel;
};

struct boss
{
    float battack;
    float bhealth;
    float bcrit;
    float bcritrate;

    int blevel;
};

struct node {
    int item_type;
    struct inventory inv;
    struct node *next;
};

void gotoxy(short x, short y)
{
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void enqueue(struct node **head, struct node **tail, struct node *temp);
void dequeue(struct node **head, struct node **tail);
void show_stat_senjata(struct senjata temp);
void show_stat_armor(struct armor temp);
void show_stat_skill(struct skill temp[], int slot);
void show_stat_char(struct character temp);
void item_database();
struct node *gacha(int rng);
void gacha_pulls(struct node **head, struct node **tail, struct character *temp);
void fight_bandits(struct character evan, struct enemy bandits, int *coins);
void fight_boss(struct character evan, struct boss yoel);
void save_data(struct character evan, int coins, int totpulls);
void load_data(struct character *evan, int *coins, int* totpulls);

int main()
{
    struct timespec current_time;
    clock_gettime(CLOCK_REALTIME, &current_time);

    srand(current_time.tv_nsec / 1000);

    struct node *Head_gacha = NULL;
    struct node *Tail_gacha = NULL;

    item_database();
    
    int coins = 0;
    int totpulls = 0;
    int pilihan = 0;
    int menuchoice;
    int enemy_choice;
    struct character evan;
    struct enemy bandits;
    struct boss yoel;

    //Main character stats
    {
        #pragma omp parallel num_threads(10)
        {
            if(omp_get_thread_num() == 0)
            {
                evan.level = 0;
            }
            if(omp_get_thread_num() == 1)
            {
                evan.base_attack = 100;
            }
            if(omp_get_thread_num() == 2)
            {
                evan.base_health = 1000;
            }
            if(omp_get_thread_num() == 3)
            {
                evan.base_crit_damage = 200;
            }
            if(omp_get_thread_num() == 4)
            {
                evan.inv.inv_senjata = Pedang;
            }
            if(omp_get_thread_num() == 5)
            {
                evan.inv.inv_armor = Baju_Kuliah;
            }
            if(omp_get_thread_num() == 6)
            {
                evan.inv.inv_skill[0] = Pukulan_Biasa;
            }
            if(omp_get_thread_num() == 7)
            {
                evan.inv.inv_skill[1] = Pukulan_Biasa;
            }
            if(omp_get_thread_num() == 8)
            {
                evan.inv.inv_skill[2] = Pukulan_Biasa;
            }
            if(omp_get_thread_num() == 9)
            {
                strcpy(evan.name, "Evan");
            }
            #pragma omp barrier
        }
    }

    //Boss stats (yoel)
    {
        #pragma omp parallel num_threads(5)
        {
            if(omp_get_thread_num() == 0)
            {
                yoel.blevel = 100;
            }
            if(omp_get_thread_num() == 1)
            {
                yoel.bhealth = 1000000;
            }
            if(omp_get_thread_num() == 2)
            {
                yoel.battack = 1500;
            }
            if(omp_get_thread_num() == 3)
            {
                yoel.bcrit = 500;
            }
            if(omp_get_thread_num() == 4)
            {
                yoel.bcritrate = 50;
            }
            #pragma omp barrier
        }
    }
    
    do
    {   
        //Bandit stats
        {
            #pragma omp parallel num_threads(5)
            {
                if(omp_get_thread_num() == 0)
                {
                    bandits.elevel = (rand() % (evan.level + 25));
                }
                #pragma omp barrier
                if(omp_get_thread_num() == 1)
                {
                    bandits.ebase_health = bandits.elevel * 200 + rand() % 1000 + 1;
                }
                if(omp_get_thread_num() == 2)
                {
                    bandits.ebase_attack = bandits.elevel * 20 + rand() % 100 + 1;
                }
                if(omp_get_thread_num() == 3)
                {
                    bandits.ebase_crit = 100 + bandits.elevel * 2 + rand() % 100 + 1;
                }
                if(omp_get_thread_num() == 4)
                {
                    bandits.ebase_critrate = rand() % 70 + 1;
                }
                #pragma omp barrier
            }
        }

        system("cls");
        printf("|=======================================|\n");
        printf("|               MAIN MENU               |\n");
        printf("|=======================================|\n");
        printf("| 1. Start                              |\n");
        printf("| 2. Save                               |\n");
        printf("| 3. Load                               |\n");
        printf("| 4. Story/Help                         |\n");
        printf("| 5. Character Stats                    |\n");
        printf("| 6. Gacha Menu                         |\n");
        printf("| 7. Exit                               |\n");
        printf("|=======================================|\n");

        do
        {
            printf("Masukkan Pilihan: ");
            scanf("%d", &menuchoice);
        } while(menuchoice < 1 || menuchoice > 7);

        switch (menuchoice)
        {
            case 1:
                {
                    system("cls");
                    printf("|=======================================|\n");
                    printf("|                  MODE                 |\n");
                    printf("|=======================================|\n");
                    printf("        1. Common Enemy (Bandits)\n");
                    printf("        2. Boss\n");
                    printf("        3. Main Menu\n");
                    printf("|=======================================|\n");

                    do
                    {
                        printf("Masukkan Pilihan: ");
                        scanf("%d", &enemy_choice);
                    }while(enemy_choice < 1 || enemy_choice > 3);

                    if(enemy_choice == 1)
                    {
                        fight_bandits(evan, bandits, &coins);
                    }
                    
                    else if(enemy_choice == 2)
                    {
                        fight_boss(evan, yoel);
                    }                        
                    
                    else
                    {
                        break;
                    }             
                }
                break;
            case 2:
                {
                    //save game
                    save_data(evan, coins, totpulls);
                }
                break;
            case 3:
                {
                    //load game
                    load_data(&evan, &coins, &totpulls);
                }
                break;
            case 4:
                {
                    system("cls");
                    printf("|===================================================================|\n");
                    printf("|                          TEKKOM FIGHTERS                          |\n");
                    printf("|===================================================================|\n");
                    printf("    Once upon a time, there lived a lone tekkom kid.\n");
                    printf(" Evan was his name and he lived a peaceful life in\n");
                    printf(" the land of UI. On a particular dark gloomy day, a\n");
                    printf(" feeling of dread overcame Evan. The computers were glitching,\n");
                    printf(" the speakers were singing in terror, Yoel The Eternal\n");
                    printf(" Space Keeper had descended from the realm of network\n");
                    printf(" with the purpose of spreading malwares and viruses. It was up\n");
                    printf(" to the humble tekkom boy, Evan, to save his lovely homeland.\n");
                    printf(" The journey of the tekkom boy, has begun.\n");
                    printf("|===================================================================|\n");
                    system("pause");
                    system("cls");
                    printf("|===============================================|\n");
                    printf("|                   HELP MENU                   |\n");
                    printf("|===============================================|\n");
                    printf("1. Game Mechanics\n");
                    printf("2. Gacha System\n");
                    int helpmenuchoice;

                    do
                    {
                        printf("Masukkan Pilihan: ");
                        scanf("%d", &helpmenuchoice);
                    }while(helpmenuchoice < 1 || helpmenuchoice > 2);

                    if(helpmenuchoice == 1)
                    {
                        printf("|===============================================|\n");
                        printf("|            WEAPONS, ARMOR, SKILLS             |\n");
                        printf("|===============================================|\n");
                        printf("   Weapon  : 10 pilihan weapon\n");
                        printf("   Armor   : 10 pilihan armor\n");
                        printf("   Skill   : 10 skill; dapat menggunakan 3 skill sekaligus dalam pertarungan\n");
                        printf("|===============================================|\n");
                        printf("|                    STATS                      |\n");
                        printf("|===============================================|\n");
                        printf("   Level            : Level player/musuh\n");
                        printf("   Base Health      : Darah standar player\n");
                        printf("   Base Attack      : Damage standar player\n");
                        printf("   Base Crit        : Damage standar critical\n");
                        printf("   Crit Chance      : Probabilitas damage critical\n");
                        printf("   Damage Multiplier: x Damage\n");
                        printf("   Max Health       : Darah maksimum yang dapat dimiliki\n");
                        printf("   HP Regeneration  : Menambah darah\n");
                        printf("   Dodge            : Menghindari serangan\n");
                        printf("   Flee             : Kabur seperti pecundang dari pertarungan\n");
                        system("pause");
                    }
                    else
                    {
                        printf("|===============================================|\n");
                        printf("|                    GACHA                      |\n");
                        printf("|===============================================|\n");
                        printf("   Sistem untuk menambah character stats (level) atau mendapatkan item (weapon, armor, skill) baru\n");
                        printf("   Gacha sistem menggunakan coins (uang) untuk spin\n");
                        system("pause");
                    }
                }
                break;
            case 5:
                {
                    system("cls");
                    show_stat_char(evan);
                    show_stat_senjata(evan.inv.inv_senjata);
                    show_stat_armor(evan.inv.inv_armor);
                    show_stat_skill(evan.inv.inv_skill, 1);
                    show_stat_skill(evan.inv.inv_skill, 2);
                    show_stat_skill(evan.inv.inv_skill, 3);
                    printf("================================================================\n");
                    system("pause");
                }
                break;
            case 6:
                {
                    system("cls");
                    printf("|=======================================|\n");
                    printf("|             ITEM DISCOVERY            |\n");
                    printf("|=======================================|\n");
                    printf("  COINS = %d\n", coins);
                    printf("  TOTAL GACHA PULLS = %d\n", totpulls);
                    printf("|=======================================|\n");
                    printf("| 1. Spin 1x   (10 Coins)               |\n");
                    printf("| 2. Spin 10x  (90 Coins)               |\n");
                    printf("| 3. Spin 20x  (180 Coins)              |\n");
                    printf("| 4. Main Menu                          |\n");
                    printf("|=======================================|\n");

                    do
                    {
                        printf("Masukkan Pilihan: ");
                        scanf("%d", &pilihan);
                        if (pilihan == 1){
                            if (coins < 10)
                            {
                                printf("\n~~~ Koin Tidak Mencukupi ~~~\n\n");
                                pilihan = 0;
                            }
                            else coins -= 10;
                        }
                        if (pilihan == 2)
                        {
                            if (coins < 90)
                            {
                                printf("\n~~~ Koin Tidak Mencukupi ~~~\n\n");
                                pilihan = 0;
                            }
                            else coins -= 90;
                        }
                        if (pilihan == 3){
                            if (coins < 180)
                            {
                                printf("\n~~~ Koin Tidak Mencukupi ~~~\n\n");
                                pilihan = 0;
                            }
                            else coins -= 180;
                        }
                        if(pilihan == 4)
                        {
                            break;
                        }
                    } while(pilihan < 1 || pilihan > 4);

                    //Gacha system menggunakan queue
                    switch (pilihan)
                    {
                        case 1:
                            {
                                int rng = rand() % 1000;
                                enqueue(&Head_gacha, &Tail_gacha, gacha(rng));
                                totpulls++;
                            }
                            break;

                        case 2:
                            {
                                int i;
                                #pragma omp parallel for reduction(+:totpulls)
                                for(i = 0; i < 10; i++)
                                {
                                    #pragma omp critical
                                    {
                                        int rng = rand() % 1000;
                                        enqueue(&Head_gacha, &Tail_gacha, gacha(rng));
                                        totpulls++;
                                    }
                                }
                            }
                            break;

                        case 3:
                            {
                                int i;
                                #pragma omp parallel shared(totpulls)
                                {
                                    #pragma omp single
                                    {
                                        for (i = 0; i < 20; i++)
                                        {
                                            #pragma omp task
                                            {
                                                int rng = rand() % 1000;
                                                enqueue(&Head_gacha, &Tail_gacha, gacha(rng));
                                                totpulls++;
                                            }   
                                            #pragma omp taskwait                                
                                        }
                                    }
                                }                       
                            }
                            break;

                        default:
                            break;
                    }
                    gacha_pulls(&Head_gacha, &Tail_gacha, &evan);
                }
                break;
            default:
                break;
        }      
    } while (menuchoice != 7);   

    return 0;
}

void enqueue(struct node **head, struct node **tail, struct node *temp)
{
    if (*head == NULL)
    {
        *head = temp;
        *tail = temp;
    }
    else
    {
        (*tail)->next = temp;
        *tail = (*tail)->next;
    }
}

void dequeue(struct node **head, struct node **tail)
{
    struct node *temp = *head;
    if (*head == *tail)
    {
        *head = NULL;
        *tail = NULL;
        free(temp);
    }
    else
    {
        *head = (*head)->next;
        free(temp);
    }
}

void show_stat_senjata(struct senjata temp)
{
    printf("================================================================\n");
    printf("                           Senjata                              \n");
    printf("================================================================\n");
    printf(" Nama                   : %s\n", temp.name);
    if (temp.rarity == 1) 
    printf(" Rarity                 : Langka\n");
    else
    printf(" Rarity                 : Legenda\n");
    printf(" Base Attack            : %.2f\n", temp.base_attack);
    printf(" Crit Rate              : %.2f %%\n", temp.crit_rate);
}

void show_stat_armor(struct armor temp)
{
    printf("================================================================\n");   
    printf("                            Armor\n");                                                     
    printf("================================================================\n");
    printf(" Nama                   : %s\n", temp.name);
    if (temp.rarity == 1)
    {
        printf(" Rarity                 : Langka\n");
    }
    else
    {
        printf(" Rarity                 : Legenda\n");
    }
    printf(" Base Health            : %.2f\n", temp.health);
    printf(" Dodge Rate             : %.2f %%\n", temp.dodge);
}

void show_stat_skill(struct skill temp[], int slot)
{
    printf("================================================================\n");
    if (slot == 0)
    {
        printf("                            Skill\n", slot);
        slot += 1;
    }
    else{
        printf("                          Skill  %d\n", slot); 
    }
                                                        
    printf("================================================================\n");
    printf(" Nama                   : %s\n", temp[slot-1].name);
    if (temp[slot-1].rarity == 1)
    {
        printf(" Rarity                 : Langka\n");
    }
    else
    {
        printf(" Rarity                 : Legenda\n");
    }
    printf(" Damage Multiplier      : %.2f %%\n", temp[slot-1].dmg_multiplier);
    printf(" Health Regen           : %.2f %%\n", temp[slot-1].hp_regenerate);
    printf(" Crit Rate Increase     : %.2f %%\n", temp[slot-1].crit_rate_inc);
    printf(" Crit Damage Increase   : %.2f %%\n", temp[slot-1].crit_damage_inc);
    printf(" Attack Increase        : %.2f\n", temp[slot-1].attack_inc);
    printf(" Health Increase        : %.2f %%\n", temp[slot-1].health_inc);
    printf(" Dodge Increase         : %.2f %%\n", temp[slot-1].dodge_inc);       
}

void show_stat_char(struct character temp){
    printf("================================================================\n");
    printf(" %s \n", temp.name);                                                        
    printf("================================================================\n");   
    printf(" Level                  : %d\n", temp.level);                               
    printf(" Base Attack            : %.2f\n", temp.base_attack);
    printf(" Attack                 : %.2f\n", temp.base_attack + temp.inv.inv_senjata.base_attack);
    printf(" Base Health            : %.2f\n", temp.base_health);
    printf(" Health                 : %.2f\n", temp.base_health + temp.inv.inv_armor.health);
    printf(" Crit Rate              : %.2f %%\n", temp.inv.inv_senjata.crit_rate);
    printf(" Crit Damage            : %.2f %%\n", temp.base_crit_damage);
    printf(" Dodge Rate             : %.2f %%\n", temp.inv.inv_armor.dodge);
                                   
    
}

struct node *gacha(int rng){
    struct node *temp = (struct node*)malloc(sizeof(struct node));
    if (rng >= 0 && rng < 730){
        temp->item_type = 0;
    }
    else if (rng >= 730 && rng < 740){
        temp->item_type = 1;
        temp->inv.inv_senjata = Pedang;
    }
    else if (rng >= 740 && rng < 750){
        temp->item_type = 1;
        temp->inv.inv_senjata = Tongkat;
    }
    else if (rng >= 750 && rng < 760){
        temp->item_type = 1;
        temp->inv.inv_senjata = Busur;
    }
    else if (rng >= 760 && rng < 770){
        temp->item_type = 1;
        temp->inv.inv_senjata = Senapan;
    }
    else if (rng >= 770 && rng < 780){
        temp->item_type = 1;
        temp->inv.inv_senjata = Pisau;
    }
    else if (rng >= 780 && rng < 790){
        temp->item_type = 1;
        temp->inv.inv_senjata = Sendal_Jepit;
    }
    else if (rng >= 790 && rng < 800){
        temp->item_type = 1;
        temp->inv.inv_senjata = Kursi_Lipat;
    }
    else if (rng >= 800 && rng < 810){
        temp->item_type = 1;
        temp->inv.inv_senjata = Peluit_Biasa;
    }
    else if (rng >= 810 && rng < 815){
        temp->item_type = 1;
        temp->inv.inv_senjata = Rocket_Launcher;
    }
    else if (rng >= 815 && rng < 820){
        temp->item_type = 1;
        temp->inv.inv_senjata = Peluit_Ultrasonik;
    }
    else if (rng >= 820 && rng < 830){
        temp->item_type = 2;
        temp->inv.inv_armor = Baju_Kuliah;
    }
    else if (rng >= 830 && rng < 840){
        temp->item_type = 2;
        temp->inv.inv_armor = Seragam_Sekolah;
    }
    else if (rng >= 840 && rng < 850){
        temp->item_type = 2;
        temp->inv.inv_armor = Baju_Tidur;
    }
    else if (rng >= 850 && rng < 860){
        temp->item_type = 2;
        temp->inv.inv_armor = Hoodie;
    }
    else if (rng >= 860 && rng < 870){
        temp->item_type = 2;
        temp->inv.inv_armor = Kemeja;
    }
    else if (rng >= 870 && rng < 880){
        temp->item_type = 2;
        temp->inv.inv_armor = Baju_Tradisional;
    }
    else if (rng >= 880 && rng < 890){
        temp->item_type = 2;
        temp->inv.inv_armor = Kaos_Dalam;
    }
    else if (rng >= 890 && rng < 900){
        temp->item_type = 2;
        temp->inv.inv_armor = Jaket_Departemen;
    }
    else if (rng >= 900 && rng < 905){
        temp->item_type = 2;
        temp->inv.inv_armor = Jas_Hitam;
    }
    else if (rng >= 905 && rng < 910){
        temp->item_type = 2;
        temp->inv.inv_armor = Jaket_Kuning;
    }
    else if (rng >= 910 && rng < 920){
        temp->item_type = 3;
        temp->inv.inv_skill[0] = Pukulan_Biasa;
    }
    else if (rng >= 920 && rng < 930){
        temp->item_type = 3;
        temp->inv.inv_skill[0] = Tendangan_Biasa;
    }
    else if (rng >= 930 && rng < 940){
        temp->item_type = 3;
        temp->inv.inv_skill[0] = Ujaran_Kebencian;
    }
    else if (rng >= 940 && rng < 950){
        temp->item_type = 3;
        temp->inv.inv_skill[0] = Pernafasan_Api;
    }
    else if (rng >= 950 && rng < 960){
        temp->item_type = 3;
        temp->inv.inv_skill[0] = Bola_Air;
    }
    else if (rng >= 960 && rng < 970){
        temp->item_type = 3;
        temp->inv.inv_skill[0] = Fitnah;
    }
    else if (rng >= 970 && rng < 980){
        temp->item_type = 3;
        temp->inv.inv_skill[0] = Tamparan_Fakta;
    }
    else if (rng >= 980 && rng < 990){
        temp->item_type = 3;
        temp->inv.inv_skill[0] = Perisai_Kebenaran;
    }
    else if (rng >= 990 && rng < 995){
        temp->item_type = 3;
        temp->inv.inv_skill[0] = Hujan_1001_Meteor;
    }
    else if (rng >= 995 && rng < 1000){
        temp->item_type = 3;
        temp->inv.inv_skill[0] = Orang_Dalam;
    }
    temp->next = NULL;
    return temp;
}

void item_database(){
    //Database Senjata

    Pedang.base_attack = 100;
    Pedang.crit_rate = 25;
    Pedang.rarity = 1;
    strcpy(Pedang.name, "Pedang");

    Tongkat.base_attack = 40;
    Tongkat.crit_rate = 60;
    Tongkat.rarity = 1;
    strcpy(Tongkat.name, "Tongkat");

    Busur.base_attack = 85;
    Busur.crit_rate = 30;
    Busur.rarity = 1;
    strcpy(Busur.name, "Busur");

    Senapan.base_attack = 120;
    Senapan.crit_rate = 10;
    Senapan.rarity = 1;
    strcpy(Senapan.name, "Senapan");

    Pisau.base_attack = 60;
    Pisau.crit_rate = 45;
    Pisau.rarity = 1;
    strcpy(Pisau.name, "Pisau");

    Sendal_Jepit.base_attack = 25;
    Sendal_Jepit.crit_rate = 80;
    Sendal_Jepit.rarity = 1;
    strcpy(Sendal_Jepit.name, "Sendal Jepit");

    Kursi_Lipat.base_attack = 250;
    Kursi_Lipat.crit_rate = 0;
    Kursi_Lipat.rarity = 1;
    strcpy(Kursi_Lipat.name, "Kursi Lipat");

    Peluit_Biasa.base_attack = 0;
    Peluit_Biasa.crit_rate = 100;
    Peluit_Biasa.rarity = 1;
    strcpy(Peluit_Biasa.name, "Peluit Biasa");

    Rocket_Launcher.base_attack = 250;
    Rocket_Launcher.crit_rate = 60;
    Rocket_Launcher.rarity = 2;
    strcpy(Rocket_Launcher.name, "Rocket Launcher");

    Peluit_Ultrasonik.base_attack = 100;
    Peluit_Ultrasonik.crit_rate = 100;
    Peluit_Ultrasonik.rarity = 2;
    strcpy(Peluit_Ultrasonik.name, "Peluit Ultrasonik");

    //Database Armor

    Baju_Kuliah.health = 1200;
    Baju_Kuliah.dodge = 5;
    Baju_Kuliah.rarity = 1;
    strcpy(Baju_Kuliah.name, "Baju Kuliah");

    Seragam_Sekolah.health = 1000;
    Seragam_Sekolah.dodge = 10;
    Seragam_Sekolah.rarity = 1;
    strcpy(Seragam_Sekolah.name, "Seragam Sekolah");

    Baju_Tidur.health = 800;
    Baju_Tidur.dodge = 15;
    Baju_Tidur.rarity = 1;
    strcpy(Baju_Tidur.name, "Baju Tidur");

    Hoodie.health = 900;
    Hoodie.dodge = 15;
    Hoodie.rarity = 1;
    strcpy(Hoodie.name, "Hoodie");

    Kemeja.health = 1400;
    Kemeja.dodge = 5;
    Kemeja.rarity = 1;
    strcpy(Kemeja.name, "Kemeja");

    Baju_Tradisional.health = 1000;
    Baju_Tradisional.dodge = 5;
    Baju_Tradisional.rarity = 1;
    strcpy(Baju_Tradisional.name, "Baju Tradisional");

    Kaos_Dalam.health = 500;
    Kaos_Dalam.dodge = 30;
    Kaos_Dalam.rarity = 1;
    strcpy(Kaos_Dalam.name, "Kaos Dalam");

    Jaket_Departemen.health = 2000;
    Jaket_Departemen.dodge = 0;
    Jaket_Departemen.rarity = 1;
    strcpy(Jaket_Departemen.name, "Jaket Departemen");

    Jas_Hitam.health = 2500;
    Jas_Hitam.dodge = 30;
    Jas_Hitam.rarity = 2;
    strcpy(Jas_Hitam.name, "Jas Hitam");

    Jaket_Kuning.health = 5000;
    Jaket_Kuning.dodge = 0;
    Jaket_Kuning.rarity = 2;
    strcpy(Jaket_Kuning.name, "Jaket Kuning");

    //Database Skill

    Pukulan_Biasa.dmg_multiplier = 100;
    Pukulan_Biasa.hp_regenerate = 15;
    Pukulan_Biasa.crit_rate_inc = 0;
    Pukulan_Biasa.crit_damage_inc = 0;
    Pukulan_Biasa.attack_inc = 0;
    Pukulan_Biasa.health_inc = 0;
    Pukulan_Biasa.dodge_inc = 0;
    Pukulan_Biasa.rarity = 1;
    strcpy(Pukulan_Biasa.name, "Pukulan Biasa");

    Tendangan_Biasa.dmg_multiplier = 135;
    Tendangan_Biasa.hp_regenerate = 0;
    Tendangan_Biasa.crit_rate_inc = 0;
    Tendangan_Biasa.crit_damage_inc = 0;
    Tendangan_Biasa.attack_inc = 20;
    Tendangan_Biasa.health_inc = 0;
    Tendangan_Biasa.dodge_inc = 0;
    Tendangan_Biasa.rarity = 1;
    strcpy(Tendangan_Biasa.name, "Tendangan Biasa");

    Ujaran_Kebencian.dmg_multiplier = 80;
    Ujaran_Kebencian.hp_regenerate = 30;
    Ujaran_Kebencian.crit_rate_inc = 5;
    Ujaran_Kebencian.crit_damage_inc = 15;
    Ujaran_Kebencian.attack_inc = 0;
    Ujaran_Kebencian.health_inc = 0;
    Ujaran_Kebencian.dodge_inc = 0;
    Ujaran_Kebencian.rarity = 1;
    strcpy(Ujaran_Kebencian.name, "Ujaran Kebencian");

    Pernafasan_Api.dmg_multiplier = 80;
    Pernafasan_Api.hp_regenerate = 30;
    Pernafasan_Api.crit_rate_inc = 0;
    Pernafasan_Api.crit_damage_inc = 15;
    Pernafasan_Api.attack_inc = 10;
    Pernafasan_Api.health_inc = 0;
    Pernafasan_Api.dodge_inc = 0;
    Pernafasan_Api.rarity = 1;
    strcpy(Pernafasan_Api.name, "Pernafasan Api");

    Bola_Air.dmg_multiplier = 80;
    Bola_Air.hp_regenerate = 30;
    Bola_Air.crit_rate_inc = 5;
    Bola_Air.crit_damage_inc = 0;
    Bola_Air.attack_inc = 10;
    Bola_Air.health_inc = 0;
    Bola_Air.dodge_inc = 0;
    Bola_Air.rarity = 1;
    strcpy(Bola_Air.name, "Bola Air");

    Fitnah.dmg_multiplier = 15;
    Fitnah.hp_regenerate = 250;
    Fitnah.crit_rate_inc = 5;
    Fitnah.crit_damage_inc = 0;
    Fitnah.attack_inc = 0;
    Fitnah.health_inc = 0;
    Fitnah.dodge_inc = 1;
    Fitnah.rarity = 1;
    strcpy(Fitnah.name, "Fitnah");

    Tamparan_Fakta.dmg_multiplier = 15;
    Tamparan_Fakta.hp_regenerate = 200;
    Tamparan_Fakta.crit_rate_inc = 0;
    Tamparan_Fakta.crit_damage_inc = 0;
    Tamparan_Fakta.attack_inc = 0;
    Tamparan_Fakta.health_inc = 30;
    Tamparan_Fakta.dodge_inc = 1;
    Tamparan_Fakta.rarity = 1;
    strcpy(Tamparan_Fakta.name, "Tamparan Fakta");

    Perisai_Kebenaran.dmg_multiplier = 0;
    Perisai_Kebenaran.hp_regenerate = 350;
    Perisai_Kebenaran.crit_rate_inc = 0;
    Perisai_Kebenaran.crit_damage_inc = 0;
    Perisai_Kebenaran.attack_inc = 0;
    Perisai_Kebenaran.health_inc = 20;
    Perisai_Kebenaran.dodge_inc = 2;
    Perisai_Kebenaran.rarity = 1;
    strcpy(Perisai_Kebenaran.name, "Perisai Kebenaran");

    Hujan_1001_Meteor.dmg_multiplier = 450;
    Hujan_1001_Meteor.hp_regenerate = 0;
    Hujan_1001_Meteor.crit_rate_inc = 5;
    Hujan_1001_Meteor.crit_damage_inc = 25;
    Hujan_1001_Meteor.attack_inc = 25;
    Hujan_1001_Meteor.health_inc = 0;
    Hujan_1001_Meteor.dodge_inc = 0;
    Hujan_1001_Meteor.rarity = 2;
    strcpy(Hujan_1001_Meteor.name, "Hujan 1001 Meteor");

    Orang_Dalam.dmg_multiplier = 0;
    Orang_Dalam.hp_regenerate = 1000;
    Orang_Dalam.crit_rate_inc = 5;
    Orang_Dalam.crit_damage_inc = 25;
    Orang_Dalam.attack_inc = 0;
    Orang_Dalam.health_inc = 100;
    Orang_Dalam.dodge_inc = 5;
    Orang_Dalam.rarity = 2;
    strcpy(Orang_Dalam.name, "Orang Dalam");
}

void gacha_pulls(struct node **head, struct node **tail, struct character *temp)
{
    while(*head != NULL)
    {
        switch((*head)->item_type){
            case 0:
                {
                    system("cls");
                    printf("================================================================\n");   
                    printf("                          Hasil Gacha                           \n");
                    printf("================================================================\n");
                    printf("                       Level Up Material                        \n");
                    printf("================================================================\n");
                    temp->level += 1;
                    temp->base_attack += 10;
                    temp->base_health += 100;
                    temp->base_crit_damage += 1;
                    show_stat_char(*temp);
                    system("pause");
                }
                break;
            case 1:
                {
                    int pilihan_gacha = 0;
                    do{
                        system("cls");
                        printf("================================================================\n");   
                        printf("                          Hasil Gacha                           \n");
                        show_stat_senjata((*head)->inv.inv_senjata);
                        printf("================================================================\n"); 
                        printf("\n\n Pilihan:\n");
                        printf("1. Tukarkan Senjata Sekarang dengan Senjata Baru\n");
                        printf("2. Simpan Senjata Sekarang\n");
                        printf("3. Lihat Senjata Sekarang\n");
                        do
                        {
                            printf("Masukkan Pilihan (1 - 4): ");
                            scanf("%d", &pilihan_gacha);
                        } while (pilihan_gacha < 1 || pilihan_gacha > 3);

                        switch (pilihan_gacha)
                        {
                            case 1:
                                temp->inv.inv_senjata = (*head)->inv.inv_senjata;
                                break;
                            case 3:
                                system("cls");
                                show_stat_senjata(temp->inv.inv_senjata);
                                printf("================================================================\n");
                                system("pause");
                                break;
                            default:
                                break;
                        }
                    }while (pilihan_gacha < 1 || pilihan_gacha > 2);
                }
                break;
            case 2:
                {
                    int pilihan_gacha = 0;
                    do
                    {
                        system("cls");
                        printf("================================================================\n");   
                        printf("                          Hasil Gacha                           \n");
                        show_stat_armor((*head)->inv.inv_armor);
                        printf("================================================================\n"); 
                        printf("\n\n Pilihan:\n");
                        printf("1. Tukarkan Armor Sekarang dengan Armor Baru\n");
                        printf("2. Simpan Armor Sekarang\n");
                        printf("3. Lihat Armor Sekarang\n");
                        do{
                            printf("Masukkan Pilihan (1 - 3): ");
                            scanf("%d", &pilihan_gacha);
                        } while (pilihan_gacha < 1 || pilihan_gacha > 3);
                        switch (pilihan_gacha)
                        {
                        case 1:
                            temp->inv.inv_armor = (*head)->inv.inv_armor;
                            break;
                        case 3:
                            system("cls");
                            show_stat_armor(temp->inv.inv_armor);
                            printf("================================================================\n");
                            system("pause");
                            break;
                        default:
                            break;
                        }
                    }while (pilihan_gacha < 1 || pilihan_gacha > 2);
                }
                break;
            case 3:
                {
                    int pilihan_gacha = 0;
                    do{
                        system("cls");
                        printf("================================================================\n");   
                        printf("                          Hasil Gacha                           \n");
                        show_stat_skill((*head)->inv.inv_skill, 0);

                        printf("\n\n Pilihan:\n");
                        printf("1. Lihat Skill Slot 1\n");
                        printf("2. Lihat Skill Slot 2\n");
                        printf("3. Lihat Skill Slot 3\n");
                        printf("4. Tukarkan Skill Slot 1 dengan Skill Baru\n");
                        printf("5. Tukarkan Skill Slot 2 dengan Skill Baru\n");
                        printf("6. Tukarkan Skill Slot 3 dengan Skill Baru\n");
                        printf("7. Buang Skill Baru\n");
                        do{
                            printf("Masukkan Pilihan (1 - 7): ");
                            scanf("%d", &pilihan_gacha);
                        } while (pilihan_gacha < 1 || pilihan_gacha > 7);
                        switch (pilihan_gacha)
                        {
                        case 1:
                            system("cls");
                            show_stat_skill(temp->inv.inv_skill, 1);
                            system("pause");
                            break;
                        case 2:
                            system("cls");
                            show_stat_skill(temp->inv.inv_skill, 2);
                            system("pause");
                            break;
                        case 3:
                            system("cls");
                            show_stat_skill(temp->inv.inv_skill, 3);
                            system("pause");
                            break;
                        case 4:
                            temp->inv.inv_skill[0] = (*head)->inv.inv_skill[0];
                            break;
                        case 5:
                            temp->inv.inv_skill[1] = (*head)->inv.inv_skill[0];
                            break;
                        case 6:
                            temp->inv.inv_skill[2] = (*head)->inv.inv_skill[0];
                            break;
                        default:
                            break;
                        }
                    }while (pilihan_gacha < 4 || pilihan_gacha > 7);
                }
                break;
            default:
                break;
        }
        dequeue(head, tail);
    }
}

void fight_bandits(struct character evan, struct enemy bandits, int *coins)
{

    int fight_choice;
    system("cls");

    float attack = evan.base_attack + evan.inv.inv_senjata.base_attack;
    float max_health = evan.base_health + evan.inv.inv_armor.health;
    float health = max_health;
    float crit_rate = evan.inv.inv_senjata.crit_rate;
    float crit_damage = evan.base_crit_damage;
    float dodge = evan.inv.inv_armor.dodge;
    float enemy_max_health = bandits.ebase_health;
    struct skill temp[3];
    temp[0] = evan.inv.inv_skill[0];
    temp[1] = evan.inv.inv_skill[1];
    temp[2] = evan.inv.inv_skill[2];

    printf("A WILD ENEMY APPEARED!\n\n");
    printf("|=======================================|\n");
    printf("|                 STATS                 |\n");
    printf("|=======================================|\n");
    printf("   Level              : %d\n", bandits.elevel);
    printf("   Health             : %.2f\n", bandits.ebase_health);
    printf("   Attack Damage      : %.2f\n", bandits.ebase_attack);
    printf("   Critical Damage    : %.2f %%\n", bandits.ebase_crit);
    printf("   Critical Rate      : %.2f %%\n", bandits.ebase_critrate);
    printf("|=======================================|\n");
    system("pause");    

    while(health > 0 && bandits.ebase_health > 0)
    {
        bandits.flee_chance = rand() % 100 + 1;
        bandits.player_flee = rand() % 100 + 1;
        
        system("cls");
        printf("|===============================================================================|\n");
        printf("|                                     FIGHT                                     |\n");
        printf("|=======================================|=======================================|\n");
        printf("|                 EVAN                  |                BANDITS                |\n");
        printf("|=======================================|=======================================|\n");
        printf("        Level      : %d                  \n", evan.level);
        printf("        Max Health : %.2f                \n", max_health);
        printf("        Health     : %.2f                \n", health);
        printf("        Attack     : %.2f                \n", attack);
        printf("        Dodge Rate : %.2f %%             \n", dodge);
        printf("        Crit Rate  : %.2f %%             \n", crit_rate);
        printf("        Crit DMG   : %.2f %%             \n", crit_damage);
        printf("|===============================================================================|\n");
        printf("        Flee Chance %.2f %%\n\n", bandits.flee_chance);
        printf("        1. Use %s\n", temp[0].name);
        printf("        2. Use %s\n", temp[1].name);
        printf("        3. Use %s\n", temp[2].name);
        printf("        4. Flee\n");
        printf("|===============================================================================|\n");


        gotoxy(40, 5);
        printf("|       Level      : %d                 ", bandits.elevel);
        gotoxy(40, 6);
        printf("|       Max Health : %.2f               ", enemy_max_health);
        gotoxy(40, 7);
        printf("|       Health     : %.2f               ", bandits.ebase_health);
        gotoxy(40, 8);
        printf("|       Attack     : %.2f               ", bandits.ebase_attack);
        gotoxy(40, 9);
        printf("|       Crit Rate  : %.2f %%            ", bandits.ebase_critrate);
        gotoxy(40, 10);
        printf("|       Crit DMG   : %.2f %%            ", bandits.ebase_crit);
        gotoxy(40, 11);
        printf("|");
        gotoxy(0, 20);
        
        do
        {
            printf("        Choice (1 - 4): ");
            scanf("%d", &fight_choice);
        } while(fight_choice < 1 || fight_choice > 4);
        system("cls");
        if(fight_choice >= 1 && fight_choice <= 3)
        {
            int crit_att = rand() % 100;
            if (crit_att <= crit_rate)
            {
                printf("|=======================================|\n");
                printf("|           EVAN CRITICAL HIT !!!       |\n");
                printf("|=======================================|\n");
                printf("|                  EVAN                 |\n");
                printf("|=======================================|\n");

                printf("        Damage Given: %.2f\n", (crit_damage/100) * (temp[fight_choice-1].dmg_multiplier/100) * attack);
                printf("        Skill: %s\n\n\n", temp[fight_choice-1].name);

                bandits.ebase_health -= (crit_damage/100) * (temp[fight_choice-1].dmg_multiplier/100) * attack;
            }
            else
            {
                printf("|=======================================|\n");
                printf("|                  EVAN                 |\n");
                printf("|=======================================|\n");
                
                printf("        Damage Given: %.2f\n", (temp[fight_choice-1].dmg_multiplier/100) * attack);
                printf("        Skill: %s\n\n\n", temp[fight_choice-1].name);

                bandits.ebase_health -= (temp[fight_choice-1].dmg_multiplier/100) * attack;
            }

            if (bandits.ebase_health <= 0){
                printf("\n\n\n");
                printf("|=======================================|\n");
                printf("|                YOU WIN                |\n");
                printf("|=======================================|\n\n");
                *coins += bandits.elevel;
                printf("You earned %d coins\n", bandits.elevel);
                system("pause");
                break;
            }

            attack += temp[fight_choice-1].attack_inc;
            max_health += (temp[fight_choice-1].health_inc/100) * attack;
            health += (temp[fight_choice-1].hp_regenerate/100) * attack;

            if (health > max_health)
            {
                health = max_health;
            }

            crit_rate += temp[fight_choice-1].crit_rate_inc;
            crit_damage += temp[fight_choice-1].crit_damage_inc;
            dodge += temp[fight_choice-1].dodge_inc;
            int dodge_att = rand() % 100;
            if (dodge > 50)
            {
                dodge = 50;
            }
            if (dodge_att <= dodge)
            {
                printf("\tDODGE ENEMY ATTACK !!!\n");
            }
            else
            {
                crit_att = rand() % 100;
                if (crit_att <= bandits.ebase_critrate)
                {
                    printf("|=======================================|\n");
                    printf("|         BANDITS CRITICAL HIT !!!      |\n");
                    printf("|=======================================|\n");
                    printf("|                BANDITS                |\n");
                    printf("|=======================================|\n");
                    printf("        Damage Given: %.2f\n", (bandits.ebase_crit/100) * bandits.ebase_attack);
                    health -= (bandits.ebase_crit/100) * bandits.ebase_attack;
                }
                else
                {
                    printf("|=======================================|\n");
                    printf("|                BANDITS                |\n");
                    printf("|=======================================|\n");
                    printf("        Damage Given: %.2f\n", bandits.ebase_attack);
                    health -= bandits.ebase_attack;
                }
            }
        }
        if (fight_choice == 4)
        {
            if(bandits.player_flee < bandits.flee_chance)
            {
                printf("|=======================================|\n");
                printf("|                ESCAPED                |\n");
                printf("|=======================================|\n");
                system("pause");
                break;
            }
            else
            {
                int crit_att = rand() % 100;
                if (crit_att <= bandits.ebase_critrate)
                {
                    printf("|=======================================|\n");
                    printf("|         BANDITS CRITICAL HIT !!!      |\n");
                    printf("|=======================================|\n");
                    printf("|                BANDITS                |\n");
                    printf("|=======================================|\n");
                    printf("        Damage Given: %.2f\n", (bandits.ebase_crit/100) * bandits.ebase_attack);
                    health -= (bandits.ebase_crit/100) * bandits.ebase_attack;
                }
                else {
                    printf("|=======================================|\n");
                    printf("|                BANDITS                |\n");
                    printf("|=======================================|\n");
                    printf("        Damage Given: %.2f\n", bandits.ebase_attack);
                    health -= bandits.ebase_attack;
                }
            }
        }

        if (health <= 0){
            printf("\n\n\n");
            printf("|=======================================|\n");
            printf("|                YOU LOSE               |\n");
            printf("|=======================================|\n");
            system("pause");
            break;
        }
        
        system("pause");
    }   
}

void fight_boss(struct character evan, struct boss yoel)
{
    int fight_choice;
    system("cls");
    float evan_attack = evan.base_attack + evan.inv.inv_senjata.base_attack;
    float evan_max_health = evan.base_health + evan.inv.inv_armor.health;
    float evan_health = evan_max_health;
    float evan_crit_rate = evan.inv.inv_senjata.crit_rate;
    float evan_crit_damage = evan.base_crit_damage;
    float evan_dodge = evan.inv.inv_armor.dodge;

    struct skill temp[3];

    temp[0] = evan.inv.inv_skill[0];
    temp[1] = evan.inv.inv_skill[1];
    temp[2] = evan.inv.inv_skill[2];

    float yoel_max_health = yoel.bhealth;
    float yoel_health = yoel_max_health;
    float yoel_attack = yoel.battack;
    float yoel_crit_rate = yoel.bcritrate;
    float yoel_crit_damage = yoel.bcrit;

    printf("THE ETERNAL SPACE KEEPER YOEL WAS SUMMONED!\n\n");
    printf("|=======================================|\n");
    printf("|                 STATS                 |\n");
    printf("|=======================================|\n");
    printf("   Level              : %d\n", yoel.blevel);
    printf("   Health             : %.2f\n", yoel.bhealth);
    printf("   Attack Damage      : %.2f\n", yoel.battack);
    printf("   Critical Damage    : %.2f %%\n", yoel.bcrit);
    printf("   Critical Rate      : %.2f %%\n", yoel.bcritrate);
    printf("|=======================================|\n");
    system("pause");

    while(evan_health > 0 && yoel_health > 0)
    {
        system("cls");
        printf("|===============================================================================|\n");
        printf("|                                     FIGHT                                     |\n");
        printf("|=======================================|=======================================|\n");
        printf("|                 EVAN                  |                  YOEL                 |\n");
        printf("|=======================================|=======================================|\n");
        printf("        Level      : %d                  \n", evan.level);
        printf("        Max Health : %.2f                \n", evan_max_health);
        printf("        Health     : %.2f                \n", evan_health);
        printf("        Attack     : %.2f                \n", evan_attack);
        printf("        Dodge Rate : %.2f %%             \n", evan_dodge);
        printf("        Crit Rate  : %.2f %%             \n", evan_crit_rate);
        printf("        Crit DMG   : %.2f %%             \n", evan_crit_damage);
        printf("|===============================================================================|\n");
        printf("        Flee Chance 0.00 %%\n\n");
        printf("        1. Use %s\n", temp[0].name);
        printf("        2. Use %s\n", temp[1].name);
        printf("        3. Use %s\n", temp[2].name);
        printf("        4. Flee\n");
        printf("|===============================================================================|\n");


        gotoxy(40, 5);
        printf("|       Level      : %d                 ", yoel.blevel);
        gotoxy(40, 6);
        printf("|       Max Health : %.2f               ", yoel.bhealth);
        gotoxy(40, 7);
        printf("|       Health     : %.2f               ", yoel_health);
        gotoxy(40, 8);
        printf("|       Attack     : %.2f               ", yoel_attack);
        gotoxy(40, 9);
        printf("|       Crit Rate  : %.2f %%            ", yoel_crit_rate);
        gotoxy(40, 10);
        printf("|       Crit DMG   : %.2f %%            ", yoel_crit_damage);
        gotoxy(40, 11);
        printf("|");
        gotoxy(0, 20); 
        
        do
        {
            printf("        Choice (1 - 4): ");
            scanf("%d", &fight_choice);
        } while(fight_choice < 1 || fight_choice > 4);
        system("cls");
        if(fight_choice >= 1 && fight_choice <= 3)
        {

            int crit_att = rand() % 100;
            if (crit_att <= evan_crit_rate)
            {
                printf("|=======================================|\n");
                printf("|           EVAN CRITICAL HIT !!!       |\n");
                printf("|=======================================|\n");
                printf("|                  EVAN                 |\n");
                printf("|=======================================|\n");

                printf("        Damage Given: %.2f\n", (evan_crit_damage/100) * (temp[fight_choice-1].dmg_multiplier/100) * evan_attack);
                printf("        Skill: %s\n\n\n", temp[fight_choice-1].name);

                yoel_health -= (evan_crit_damage/100) * (temp[fight_choice-1].dmg_multiplier/100) * evan_attack;
            }
            else
            {
                printf("|=======================================|\n");
                printf("|                  EVAN                 |\n");
                printf("|=======================================|\n");
                
                printf("        Damage Given: %.2f\n", (temp[fight_choice-1].dmg_multiplier/100) * evan_attack);
                printf("        Skill: %s\n\n\n", temp[fight_choice].name);

                yoel_health -= (temp[fight_choice-1].dmg_multiplier/100) * evan_attack;
            }

            if (yoel_health <= 0){
                printf("\n\n\n");
                printf("|=======================================|\n");
                printf("|                YOU WIN                |\n");
                printf("|=======================================|\n");
                system("pause");
                break;
            }
            evan_attack += temp[fight_choice-1].attack_inc;
            evan_max_health += (temp[fight_choice-1].health_inc/100) * evan_attack;
            evan_health += (temp[fight_choice-1].hp_regenerate/100) * evan_attack;

            if (evan_health > evan_max_health)
            {
                evan_health = evan_max_health;
            }

            evan_crit_rate += temp[fight_choice-1].crit_rate_inc;
            evan_crit_damage += temp[fight_choice-1].crit_damage_inc;
            evan_dodge += temp[fight_choice-1].dodge_inc;
            int dodge_att = rand() % 100;
            if (evan_dodge > 50)
            {
                evan_dodge = 50;
            }
            if (dodge_att <= evan_dodge)
            {
                printf("\tDODGE YOEL ATTACK !!!\n");
            }
            else
            {
                crit_att = rand() % 100;
                if (crit_att <= yoel_crit_rate)
                {
                    printf("|=======================================|\n");
                    printf("|          YOEL CRITICAL HIT !!!        |\n");
                    printf("|=======================================|\n");
                    printf("|                 YOEL                  |\n");
                    printf("|=======================================|\n");
                    printf("        Damage Given: %.2f\n", (yoel_crit_damage/100) * yoel_attack);
                    evan_health -= (yoel_crit_damage/100) * yoel_attack;
                }
                else
                {
                    printf("|=======================================|\n");
                    printf("|                 YOEL                  |\n");
                    printf("|=======================================|\n");
                    printf("        Damage Given: %.2f\n", yoel_attack);
                    evan_health -= yoel_attack;
                }
            }
        }
        if (fight_choice == 4)
        {
            int crit_att = rand() % 100;
            if (crit_att <= yoel_crit_rate)
            {
                printf("|=======================================|\n");
                printf("|          YOEL CRITICAL HIT !!!        |\n");
                printf("|=======================================|\n");
                printf("|                 YOEL                  |\n");
                printf("|=======================================|\n");
                printf("        Damage Given: %.2f\n", (yoel_crit_damage/100) * yoel_attack);
                evan_health -= (yoel_crit_damage/100) * yoel_attack;
            }
            else
            {
                printf("|=======================================|\n");
                printf("|                 YOEL                  |\n");
                printf("|=======================================|\n");
                printf("        Damage Given: %.2f\n", yoel_attack);
                evan_health -= yoel_attack;
            }
        }

        if (evan_health <= 0){
            printf("\n\n\n");
            printf("|=======================================|\n");
            printf("|               YOU LOSE                |\n");
            printf("|=======================================|\n");
            system("pause");
            break;
        }
        
        system("pause");
    } 
}

void save_data(struct character evan, int coins, int totpulls)
{
    FILE *fptr;
    fptr = fopen("Data.txt", "w");
    // Save Coins
    fprintf(fptr, "%d\n", coins);
    fprintf(fptr, "%d\n", totpulls);

    //Save Character stats
    fprintf(fptr, "%.2f\n", evan.base_attack);
    fprintf(fptr, "%.2f\n", evan.base_health);
    fprintf(fptr, "%.2f\n", evan.base_crit_damage);
    fprintf(fptr, "%d\n", evan.level);
    fprintf(fptr, "%s\n", evan.name);

    //Save Character Weapon
    fprintf(fptr, "%.2f\n", evan.inv.inv_senjata.base_attack);
    fprintf(fptr, "%.2f\n", evan.inv.inv_senjata.crit_rate);
    fprintf(fptr, "%d\n", evan.inv.inv_senjata.rarity);
    fprintf(fptr, "%s\n", evan.inv.inv_senjata.name);

    //Save Character Armor
    fprintf(fptr, "%.2f\n", evan.inv.inv_armor.health);
    fprintf(fptr, "%.2f\n", evan.inv.inv_armor.dodge);
    fprintf(fptr, "%d\n", evan.inv.inv_armor.rarity);
    fprintf(fptr, "%s\n", evan.inv.inv_armor.name);

    //Save Skill 1
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[0].dmg_multiplier);
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[0].hp_regenerate);
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[0].crit_rate_inc);
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[0].crit_damage_inc);
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[0].attack_inc);
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[0].health_inc);
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[0].dodge_inc);
    fprintf(fptr, "%d\n", evan.inv.inv_skill[0].rarity);
    fprintf(fptr, "%s\n", evan.inv.inv_skill[0].name);

    //Save Skill 2
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[1].dmg_multiplier);
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[1].hp_regenerate);
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[1].crit_rate_inc);
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[1].crit_damage_inc);
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[1].attack_inc);
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[1].health_inc);
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[1].dodge_inc);
    fprintf(fptr, "%d\n", evan.inv.inv_skill[1].rarity);
    fprintf(fptr, "%s\n", evan.inv.inv_skill[1].name);

    //Save Skill 3
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[2].dmg_multiplier);
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[2].hp_regenerate);
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[2].crit_rate_inc);
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[2].crit_damage_inc);
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[2].attack_inc);
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[2].health_inc);
    fprintf(fptr, "%.2f\n", evan.inv.inv_skill[2].dodge_inc);
    fprintf(fptr, "%d\n", evan.inv.inv_skill[2].rarity);
    fprintf(fptr, "%s\n", evan.inv.inv_skill[2].name);

    fclose(fptr);

    system("cls");
    printf("|=======================================|\n");
    printf("|               Data Saved              |\n");
    printf("|=======================================|\n");
    system("pause");
}

void load_data(struct character *evan, int *coins, int *totpulls)
{
    FILE *fptr;

    fptr = fopen("Data.txt", "r");
    if (fptr == NULL)
    {
        system("cls");
        printf("|=======================================|\n");
        printf("|          Save File Not Found          |\n");
        printf("|=======================================|\n");
        system("pause");
    }
    else
    {
        //Load Coins from Data
        fscanf(fptr, "%d", coins);
        fscanf(fptr, "%d", totpulls);

        //Load Character Stats from Data
        fscanf(fptr, "%f", &evan->base_attack);
        fscanf(fptr, "%f", &evan->base_health);
        fscanf(fptr, "%f", &evan->base_crit_damage);
        fscanf(fptr, "%d", &evan->level);
        fscanf(fptr, " %[^\n]s", evan->name);

        //Load Character Weapon from Data
        fscanf(fptr, "%f", &evan->inv.inv_senjata.base_attack);
        fscanf(fptr, "%f", &evan->inv.inv_senjata.crit_rate);
        fscanf(fptr, "%d", &evan->inv.inv_senjata.rarity);
        fscanf(fptr, " %[^\n]s", evan->inv.inv_senjata.name);

        //Load Character Armor from Data
        fscanf(fptr, "%f", &evan->inv.inv_armor.health);
        fscanf(fptr, "%f", &evan->inv.inv_armor.dodge);
        fscanf(fptr, "%d", &evan->inv.inv_armor.rarity);
        fscanf(fptr, " %[^\n]s", evan->inv.inv_armor.name);

        //Load Character Skill 1 from Data
        fscanf(fptr, "%f", &evan->inv.inv_skill[0].dmg_multiplier);
        fscanf(fptr, "%f", &evan->inv.inv_skill[0].hp_regenerate);
        fscanf(fptr, "%f", &evan->inv.inv_skill[0].crit_rate_inc);
        fscanf(fptr, "%f", &evan->inv.inv_skill[0].crit_damage_inc);
        fscanf(fptr, "%f", &evan->inv.inv_skill[0].attack_inc);
        fscanf(fptr, "%f", &evan->inv.inv_skill[0].health_inc);
        fscanf(fptr, "%f", &evan->inv.inv_skill[0].dodge_inc);
        fscanf(fptr, "%d", &evan->inv.inv_skill[0].rarity);
        fscanf(fptr, " %[^\n]s", evan->inv.inv_skill[0].name);

        //Load Character Skill 2 from Data
        fscanf(fptr, "%f", &evan->inv.inv_skill[1].dmg_multiplier);
        fscanf(fptr, "%f", &evan->inv.inv_skill[1].hp_regenerate);
        fscanf(fptr, "%f", &evan->inv.inv_skill[1].crit_rate_inc);
        fscanf(fptr, "%f", &evan->inv.inv_skill[1].crit_damage_inc);
        fscanf(fptr, "%f", &evan->inv.inv_skill[1].attack_inc);
        fscanf(fptr, "%f", &evan->inv.inv_skill[1].health_inc);
        fscanf(fptr, "%f", &evan->inv.inv_skill[1].dodge_inc);
        fscanf(fptr, "%d", &evan->inv.inv_skill[1].rarity);
        fscanf(fptr, " %[^\n]s", evan->inv.inv_skill[1].name);

        //Load Character Skill 3 from Data
        fscanf(fptr, "%f", &evan->inv.inv_skill[2].dmg_multiplier);
        fscanf(fptr, "%f", &evan->inv.inv_skill[2].hp_regenerate);
        fscanf(fptr, "%f", &evan->inv.inv_skill[2].crit_rate_inc);
        fscanf(fptr, "%f", &evan->inv.inv_skill[2].crit_damage_inc);
        fscanf(fptr, "%f", &evan->inv.inv_skill[2].attack_inc);
        fscanf(fptr, "%f", &evan->inv.inv_skill[2].health_inc);
        fscanf(fptr, "%f", &evan->inv.inv_skill[2].dodge_inc);
        fscanf(fptr, "%d", &evan->inv.inv_skill[2].rarity);
        fscanf(fptr, " %[^\n]s", evan->inv.inv_skill[2].name);

        system("cls");
        printf("|=======================================|\n");
        printf("|              Data Loaded              |\n");
        printf("|=======================================|\n");
        system("pause");
    }

    fclose(fptr);
}