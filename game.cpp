//
// Created by 28339 on 2020/3/30.
//
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include <pthread.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib") //加载 ws2_32.dll
#define BUF_SIZE 100

#define PI 3.1415926

// 引用 Windows Multimedia API
#pragma comment(lib,"Winmm.lib")

const int High = 600;
const int Width = 800;
int map_x;
int map_y;       //地图坐标
int wall_x_a, wall_x_b;
int wall_y_a, wall_y_b;   //记录墙的范围

int pvp;
int hit_enemy;    // 打中敌人的血量

int player_x;
int player_y;  //玩家坐标
int jue_player_x;
int jue_player_y;      // 玩家绝对位置
int player_walk;       //玩家行走动作控制
int player_hp;  // 玩家血量

int dazhao;       // 玩家技能
int dazhao_xuli;     // 蓄力
int dazhao_need;
int dazhao_time;   // 剩余时间
int dazhao_time_limit; // 持续时间

int gun_model;     // 枪的型号
int fire;    //开火状态
int hit;      // 标记是否打到僵尸
int gun_change;    // 标记是否换枪
int gun_model_old;   // 前一把枪
int gun_level; // 解锁枪的等级
struct gun
{
    int bullet;     // 子弹数量
    int bullet_max;   // 子弹上限
    int power;      // 子弹威力
    int fire_length;    // 子弹长度
    int fire_time;    //子弹可见时间
    int fire_time_count;
    int gun_stop;       //枪射速延迟
    int gun_speed;      // 枪射速
    int gun_music; // 枪声时间
    int gun_music_count;
}gun_present,handgun,shotgun, uzi;

char player_direction;   // 玩家方向

int level;   // 等级
int combo;    //连击
int combo_time;   // 连击时间
int combo_time_limit;      // 最长连击时间
int score;     // 分数

const int redbox_num_limit = 10;    //最多10个红盒子
int redbox_num = 4;    // 初始4个红盒子
IMAGE img_redbox;
IMAGE img_redbox_cover;
struct redbox   // 定义红盒子
{
    int exist;        // 存在
    int x;
    int y;             // 坐标
    int pick;
}rb[redbox_num_limit];

const int zombie_num_limit = 100; // 僵尸最大数量100
int zombie_num;                 // 使用僵尸数量
int zombie_x[zombie_num_limit];
int zombie_y[zombie_num_limit];   // 僵尸坐标
int zombie_alive[zombie_num_limit];   // 僵尸存活状态
char zombie_move_direction[zombie_num_limit];    //僵尸移动方向
const int zombie_move_speed = 10;     //僵尸移动速度：几次循环移动一次
int zombie_move_count[zombie_num_limit];    // 控制移动
const int zombie_attack_speed = 20;
int zombie_attack_count[zombie_num_limit];      // 控制僵尸攻击
int attack[zombie_num_limit];    // 检测是否在攻击状态
int zombie_hp[zombie_num_limit];  //僵尸血量
int zombie_walk[zombie_num_limit];     //僵尸走路状态控制
int zombie_walk_count[zombie_num_limit];     //僵尸走路状态控制 用于减速

int enemy_x;
int enemy_y;
int enemy_walk;
int enemy_hp;            // 定义敌人信息
char enemy_direction;      //敌人方向
int enemy_gun_model;
int enemy_fire;   // 敌人开火状态
int enemy_fire_len;

int start;   // 开始游戏
int pick_start;    // 菜单选项条选择
int pick_tips;
int pick_options;
int pick_pvp;
int pick_continue;
int pick_rank;
int pick_quit;
int pick_create;
int pick_join;
int choose_pvp;
int pick_backtomenu;

int bkmusic;   // 背景音乐开关
int sound_switch;   // 音效开关
int volume;    // 音量
int volume_control_x;     //音量控制按钮位置
int volume_x;            //音量条位置
int ctrl_volume;               // 音量条控制状态
int sound;    // 音效
int sound_control_x;     //音效控制按钮位置
int sound_x;            //音效条位置
int ctrl_sound;               // 音效条控制状态

int pause_exit;     // 退出暂停状态

int outmap_x, outmap_y;  //图片移动到边时控制人物移动
MOUSEMSG m;  // 鼠标控制

IMAGE img_map;//地图
IMAGE img_wall;  //墙壁
IMAGE img_wall_cover;

IMAGE img_menu_background; // 菜单背景
IMAGE img_singleplay;   // 选项条
IMAGE img_singleplay_cover;
IMAGE img_singleplay_pick;
IMAGE img_tips;
IMAGE img_tips_cover;
IMAGE img_tips_pick;
IMAGE img_tips_picture;     //提示页面图片
IMAGE img_continue;
IMAGE img_continue_pick;
IMAGE img_continue_cover;
IMAGE img_pvp;
IMAGE img_pvp_cover;
IMAGE img_pvp_pick;
IMAGE img_rank;
IMAGE img_rank_cover;
IMAGE img_rank_pick;
IMAGE img_quit;
IMAGE img_quit_cover;
IMAGE img_quit_pick;
IMAGE img_create;
IMAGE img_create_cover;
IMAGE img_create_pick;
IMAGE img_join;
IMAGE img_join_cover;
IMAGE img_join_pick;
IMAGE img_backtomenu;
IMAGE img_backtomenu_pick;
IMAGE img_backtomenu_cover;

IMAGE img_options;
IMAGE img_options_cover;
IMAGE img_options_pick;
IMAGE img_volume;
IMAGE img_volume_control;
IMAGE img_volume_button;     // 音量按键控制背景音乐开关
IMAGE img_volume_button_cover;
IMAGE img_volume_button_pick;
IMAGE img_volume_button_pick_cover;
IMAGE img_sound_button;     // 音效按键控制背景音乐开关
IMAGE img_sound_button_cover;
IMAGE img_sound_button_pick;
IMAGE img_sound_button_pick_cover;
IMAGE img_back;      //返回按键
IMAGE img_back_cover;
IMAGE img_back_pick;
IMAGE img_back_pick_cover;

IMAGE img_pause_background;   //暂停背景

IMAGE img_loading;   //加载图片
IMAGE img_loading_bar;    // 进度条
IMAGE img_little_loading;
IMAGE img_little_loading_bk;
IMAGE img_little_loading2;  // 旋转use

IMAGE img_player;
IMAGE img_player_cover;
IMAGE img_hp;       // 玩家血量
IMAGE img_hp_cover;
IMAGE img_w;    //  走路图片组
IMAGE img_a;
IMAGE img_s;
IMAGE img_d;
IMAGE img_w_cover;    //  走路图片组
IMAGE img_a_cover;
IMAGE img_s_cover;
IMAGE img_d_cover;

IMAGE img_gun;        // 定义枪的图片
IMAGE img_gun_cover;
IMAGE img_gun_a;
IMAGE img_gun_a_cover;
IMAGE img_gun_d;
IMAGE img_gun_d_cover;
IMAGE img_gun_s;
IMAGE img_gun_s_cover;
IMAGE img_gun_w;
IMAGE img_gun_w_cover;
IMAGE img_shotgun_a;           //加载shotgun图片
IMAGE img_shotgun_a_cover;
IMAGE img_shotgun_d;
IMAGE img_shotgun_d_cover;
IMAGE img_shotgun_s;
IMAGE img_shotgun_s_cover;
IMAGE img_shotgun_w;
IMAGE img_shotgun_w_cover;
IMAGE img_uzi_a;           //加载uzi图片
IMAGE img_uzi_a_cover;
IMAGE img_uzi_d;
IMAGE img_uzi_d_cover;
IMAGE img_uzi_s;
IMAGE img_uzi_s_cover;
IMAGE img_uzi_w;
IMAGE img_uzi_w_cover;
IMAGE img_handgun_a;           //加载handgun图片
IMAGE img_handgun_a_cover;
IMAGE img_handgun_d;
IMAGE img_handgun_d_cover;
IMAGE img_handgun_s;
IMAGE img_handgun_s_cover;
IMAGE img_handgun_w;
IMAGE img_handgun_w_cover;

IMAGE img_dazhao;

IMAGE img_zombie[zombie_num_limit];              //僵尸图片组
IMAGE img_zombie_cover[zombie_num_limit];              //僵尸图片组
IMAGE img_zombie_blood;  //僵尸血
IMAGE img_zombie_blood_cover;
IMAGE img_zombie_a;
IMAGE img_zombie_a_cover;
IMAGE img_zombie_s;
IMAGE img_zombie_s_cover;                       //各个方向
IMAGE img_zombie_d;
IMAGE img_zombie_d_cover;
IMAGE img_zombie_w;
IMAGE img_zombie_w_cover;
IMAGE img_zombie_aw;
IMAGE img_zombie_aw_cover;
IMAGE img_zombie_as;
IMAGE img_zombie_as_cover;
IMAGE img_zombie_ds;
IMAGE img_zombie_ds_cover;
IMAGE img_zombie_dw;
IMAGE img_zombie_dw_cover;

IMAGE img_gameover;     // gameover背景

IMAGE img_enemy;
IMAGE img_enemy_cover;
IMAGE img_enemy_hp;         //   敌人血量
IMAGE img_enemy_hp_cover;
IMAGE img_enemy_w;          //  走路图片组
IMAGE img_enemy_a;
IMAGE img_enemy_s;
IMAGE img_enemy_d;
IMAGE img_enemy_w_cover;    //  走路图片组
IMAGE img_enemy_a_cover;
IMAGE img_enemy_s_cover;
IMAGE img_enemy_d_cover;
IMAGE img_enemy_gun;
IMAGE img_enemy_gun_cover;
IMAGE img_enemy_gun_a;
IMAGE img_enemy_gun_a_cover;
IMAGE img_enemy_gun_d;
IMAGE img_enemy_gun_d_cover;
IMAGE img_enemy_gun_s;
IMAGE img_enemy_gun_s_cover;
IMAGE img_enemy_gun_w;
IMAGE img_enemy_gun_w_cover;

IMAGE img_0;
IMAGE img_0_cover;
IMAGE img_1;
IMAGE img_1_cover;
IMAGE img_2;
IMAGE img_2_cover;
IMAGE img_3;
IMAGE img_3_cover;
IMAGE img_4;
IMAGE img_4_cover;
IMAGE img_5;
IMAGE img_5_cover;
IMAGE img_6;
IMAGE img_6_cover;
IMAGE img_7;
IMAGE img_7_cover;
IMAGE img_8;
IMAGE img_8_cover;
IMAGE img_9;
IMAGE img_9_cover;
IMAGE img_num[10];
IMAGE img_num_cover[10];

IMAGE img_s0;
IMAGE img_s0_cover;
IMAGE img_s1;
IMAGE img_s1_cover;
IMAGE img_s2;
IMAGE img_s2_cover;
IMAGE img_s3;
IMAGE img_s3_cover;
IMAGE img_s4;
IMAGE img_s4_cover;
IMAGE img_s5;
IMAGE img_s5_cover;
IMAGE img_s6;
IMAGE img_s6_cover;
IMAGE img_s7;
IMAGE img_s7_cover;
IMAGE img_s8;
IMAGE img_s8_cover;
IMAGE img_s9;
IMAGE img_s9_cover;
IMAGE img_snum[10];
IMAGE img_snum_cover[10];
IMAGE img_combo;
IMAGE img_combo_cover;
IMAGE img_combo_x;
IMAGE img_combo_x_cover;
IMAGE img_combo_p;
IMAGE img_combo_p_cover;

IMAGE img_rank_pic;

/*==================================线程启动+敌人更新模块=================================*/
void startup_pvp()  // 开启pvp模式
{

}

void* tprocess2(void* args)//客户端线程启动*******************************************
{
    //初始化DLL127.0.0.1
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //创建套接字
    SOCKET sock = socket(PF_INET, SOCK_DGRAM, 0);
    //服务器地址信息
    sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));  //每个字节都用0填充
    servAddr.sin_family = PF_INET;
    char sip[20];
    InputBox(sip, 20, "请输入服务器ip");
    servAddr.sin_addr.s_addr = inet_addr(sip);
    servAddr.sin_port = htons(1234);
    //不断获取用户输入并发送给服务器，然后接受服务器数据
    sockaddr fromAddr;
    int addrLen = sizeof(fromAddr);
    char x[BUF_SIZE];  //缓冲区
    char y[BUF_SIZE];  //缓冲区
    char h[BUF_SIZE];  //缓冲区 击打
    char hp[BUF_SIZE];  //缓冲区 血量
    char d[BUF_SIZE];   // 方向
    char g[BUF_SIZE];    // 玩家用枪
    char f[BUF_SIZE];    // 开火状态

    char shou[BUF_SIZE];  //缓冲区
    char fa[BUF_SIZE];  //缓冲区 发包

    char e_x[BUF_SIZE];  //缓冲区
    char e_y[BUF_SIZE];  //缓冲区
    char e_h[BUF_SIZE];  //缓冲区
    char e_hp[BUF_SIZE];  //缓冲区
    char e_d[BUF_SIZE];    //敌人方向
    char e_g[BUF_SIZE];    // 敌人用枪
    char e_f[BUF_SIZE];    // 开火状态

    char duan[] = "$";
    int i;
    int j;

    while(1){
        sprintf(x, "%d" , player_x);
        sprintf(y, "%d" , player_y);
        sprintf(hp, "%d" , player_hp);
        sprintf(h, "%d" , hit_enemy);
        d[0] = player_direction;
        d[1] = '\0';
        sprintf(g, "%d" , gun_model);
        sprintf(f, "%d" , fire);
        hit_enemy = 0;

        strcpy(fa, x);
        strcat(fa, duan);
        strcat(fa, y);
        strcat(fa, duan);
        strcat(fa, h);
        strcat(fa, duan);
        strcat(fa, hp);
        strcat(fa, duan);
        strcat(fa, d);
        strcat(fa, duan);
        strcat(fa, g);
        strcat(fa, duan);
        strcat(fa, f);
        strcat(fa, duan);

        sendto(sock, fa, strlen(fa) + 10, 0 , (struct sockaddr*)&servAddr, sizeof(servAddr));    // 发送信息

        recvfrom(sock, shou, BUF_SIZE, 0, &fromAddr, &addrLen);  // 获取敌人的信息并更新

        i = 0;
        j = 0;
        while (shou[i] != '$'){
            e_x[j] = shou[i];
            j ++;
            i ++;
        }
        e_x[j] = '\0';
        i ++;
        j = 0;
        while (shou[i] != '$'){
            e_y[j] = shou[i];
            j ++;
            i ++;
        }
        e_y[j] = '\0';
        i ++;
        j = 0;
        while (shou[i] != '$'){
            e_h[j] = shou[i];
            j ++;
            i ++;
        }
        e_h[j] = '\0';
        i ++;
        j = 0;
        while (shou[i] != '$'){
            e_hp[j] = shou[i];
            j ++;
            i ++;
        }
        e_hp[j] = '\0';
        i ++;
        j = 0;
        while (shou[i] != '$'){
            e_d[j] = shou[i];
            j ++;
            i ++;
        }
        e_d[j] = '\0';
        i ++;
        j = 0;
        while (shou[i] != '$'){
            e_g[j] = shou[i];
            j ++;
            i ++;
        }
        e_g[j] = '\0';
        i ++;
        j = 0;
        while (shou[i] != '$'){
            e_f[j] = shou[i];
            j ++;
            i ++;
        }
        e_f[j] = '\0';

        enemy_x = atoi (e_x);
        enemy_y = atoi (e_y);
        player_hp = player_hp - atoi (e_h);
        enemy_hp = atoi (e_hp);
        enemy_direction = e_d[0];
        enemy_gun_model = atoi (e_g);
        enemy_fire = atoi (e_f);

    }
    closesocket(sock);
    WSACleanup();
    return 0;

}

void* tprocess1(void* args)//服务器线程启动*******************************************
{
    WSADATA wsaData;
    WSAStartup( MAKEWORD(2, 2), &wsaData);
    //创建套接字
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    //绑定套接字
    sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));  //每个字节都用0填充
    servAddr.sin_family = PF_INET;  //使用IPv4地址
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); //自动获取IP地址
    servAddr.sin_port = htons(1234);  //端口
    bind(sock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR));
    //接收客户端请求
    SOCKADDR clntAddr;  //客户端地址信息
    int nSize = sizeof(SOCKADDR);

    char x[BUF_SIZE];  //缓冲区
    char y[BUF_SIZE];  //缓冲区
    char h[BUF_SIZE];  //缓冲区 击打
    char hp[BUF_SIZE];  //缓冲区 血量
    char d[BUF_SIZE];   // 方向
    char g[BUF_SIZE];    // 玩家用枪
    char f[BUF_SIZE];    // 开火状态

    char shou[BUF_SIZE];  //缓冲区
    char fa[BUF_SIZE];  //缓冲区 发包

    char e_x[BUF_SIZE];  //缓冲区
    char e_y[BUF_SIZE];  //缓冲区
    char e_h[BUF_SIZE];  //缓冲区
    char e_hp[BUF_SIZE];  //缓冲区
    char e_d[BUF_SIZE];    //敌人方向
    char e_g[BUF_SIZE];    // 敌人用枪
    char e_f[BUF_SIZE];    // 开火状态

    char duan[] = "$";
    int i;
    int j;
    while(1){
        sprintf(x, "%d" , player_x);
        sprintf(y, "%d" , player_y);
        sprintf(hp, "%d" , player_hp);
        sprintf(h, "%d" , hit_enemy);
        d[0] = player_direction;
        d[1] = '\0';
        sprintf(g, "%d" , gun_model);
        sprintf(f, "%d" , fire);
        hit_enemy = 0;

        strcpy(fa, x);
        strcat(fa, duan);
        strcat(fa, y);
        strcat(fa, duan);
        strcat(fa, h);
        strcat(fa, duan);
        strcat(fa, hp);
        strcat(fa, duan);
        strcat(fa, d);
        strcat(fa, duan);
        strcat(fa, g);
        strcat(fa, duan);
        strcat(fa, f);
        strcat(fa, duan);

        sendto(sock, fa, strlen(fa) + 10, 0 ,&clntAddr, nSize);    // 发送自己的位置

        recvfrom(sock, shou, BUF_SIZE, 0, &clntAddr, &nSize);  // 获取敌人的位置信息并更新

        i = 0;
        j = 0;
        while (shou[i] != '$'){
            e_x[j] = shou[i];
            j ++;
            i ++;
        }
        e_x[j] = '\0';
        i ++;
        j = 0;
        while (shou[i] != '$'){
            e_y[j] = shou[i];
            j ++;
            i ++;
        }
        e_y[j] = '\0';
        i ++;
        j = 0;
        while (shou[i] != '$'){
            e_h[j] = shou[i];
            j ++;
            i ++;
        }
        e_h[j] = '\0';
        i ++;
        j = 0;
        while (shou[i] != '$'){
            e_hp[j] = shou[i];
            j ++;
            i ++;
        }
        e_hp[j] = '\0';
        i ++;
        j = 0;
        while (shou[i] != '$'){
            e_d[j] = shou[i];
            j ++;
            i ++;
        }
        e_d[j] = '\0';
        i ++;
        j = 0;
        while (shou[i] != '$'){
            e_g[j] = shou[i];
            j ++;
            i ++;
        }
        e_g[j] = '\0';
        i ++;
        j = 0;
        while (shou[i] != '$'){
            e_f[j] = shou[i];
            j ++;
            i ++;
        }
        e_f[j] = '\0';

        enemy_x = atoi (e_x);
        enemy_y = atoi (e_y);
        player_hp = player_hp - atoi (e_h);
        enemy_hp = atoi (e_hp);
        enemy_direction = e_d[0];
        enemy_gun_model = atoi (e_g);
        enemy_fire = atoi (e_f);

    }
    closesocket(sock);
    WSACleanup();
    return NULL;
}
/*=============================================初始化===================================================*/
/*=============================================初始化===================================================*/

void initialize_enemy()     // 初始化敌人
{
    enemy_x = 0;
    enemy_y = 0;
    enemy_hp = 100;
    enemy_direction = 'w';
    enemy_walk = 0;
}


void initialize_handgun()//初始化handgun-------1
{
    handgun.power = 5;
    handgun.fire_length = 300;
    handgun.fire_time = 20;
    handgun.fire_time_count = 0;
    handgun.bullet_max = 60;
    handgun.bullet = handgun.bullet_max;
    handgun.gun_stop = 0;
    handgun.gun_speed = 200;         // 射速
    handgun.gun_music = 20;
    handgun.gun_music_count = 0;
}

void initialize_shotgun()//初始化shotgun-------3
{
    shotgun.power = 10;
    shotgun.fire_length = 200;
    shotgun.fire_time = 20;
    shotgun.fire_time_count = 0;
    shotgun.bullet_max = 40;
    shotgun.bullet = shotgun.bullet_max;
    shotgun.gun_stop = 0;
    shotgun.gun_speed = 350;         // 射速
    shotgun.gun_music = 20;
    shotgun.gun_music_count = 0;
}

void initialize_uzi()//初始化uzi------2
{
    uzi.power = 3;
    uzi.fire_length = 500;
    uzi.fire_time = 20;       // 火焰显示时间（未调试）
    uzi.fire_time_count = 0;
    uzi.bullet_max = 250;
    uzi.bullet = uzi.bullet_max;
    uzi.gun_stop = 0;
    uzi.gun_speed = 50;         // 射速
    uzi.gun_music = 150;
    uzi.gun_music_count = 0;
}

void load_img()
{
    loadimage (&img_map, ".\\pic\\map1.jpg");
    loadimage (&img_wall, ".\\pic\\wall.jpg");
    loadimage (&img_wall_cover, ".\\pic\\wall_cover.jpg");

    loadimage (&img_menu_background, ".\\pic\\menu_background.jpg");
    loadimage (&img_singleplay, ".\\pic\\singleplay.jpg");
    loadimage (&img_singleplay_cover, ".\\pic\\singleplay_cover.jpg");
    loadimage (&img_singleplay_pick, ".\\pic\\singleplay_pick.jpg");
    loadimage (&img_tips, ".\\pic\\tips.jpg");
    loadimage (&img_tips_cover, ".\\pic\\tips_cover.jpg");
    loadimage (&img_tips_pick, ".\\pic\\tips_pick.jpg");
    loadimage (&img_tips_picture, ".\\pic\\tips_picture.jpg");
    loadimage (&img_pvp, ".\\pic\\pvp.jpg");
    loadimage (&img_pvp_cover, ".\\pic\\pvp_cover.jpg");
    loadimage (&img_pvp_pick, ".\\pic\\pvp_pick.jpg");
    loadimage (&img_rank, ".\\pic\\rank.jpg");
    loadimage (&img_rank_cover, ".\\pic\\rank_cover.jpg");
    loadimage (&img_rank_pick, ".\\pic\\rank_pick.jpg");
    loadimage (&img_quit, ".\\pic\\quit.jpg");
    loadimage (&img_quit_cover, ".\\pic\\quit_cover.jpg");
    loadimage (&img_quit_pick, ".\\pic\\quit_pick.jpg");
    loadimage (&img_create, ".\\pic\\create.jpg");
    loadimage (&img_create_cover, ".\\pic\\create_cover.jpg");
    loadimage (&img_create_pick, ".\\pic\\create_pick.jpg");
    loadimage (&img_join, ".\\pic\\join.jpg");
    loadimage (&img_join_cover, ".\\pic\\join_cover.jpg");
    loadimage (&img_join_pick, ".\\pic\\join_pick.jpg");
    loadimage (&img_backtomenu, ".\\pic\\backtomenu.jpg");
    loadimage (&img_backtomenu_cover, ".\\pic\\backtomenu_cover.jpg");
    loadimage (&img_backtomenu_pick, ".\\pic\\backtomenu_pick.jpg");

    loadimage (&img_enemy_hp, ".\\pic\\enemy_hp.jpg");

    loadimage (&img_continue, ".\\pic\\continue.jpg");
    loadimage (&img_continue_cover, ".\\pic\\continue_cover.jpg");
    loadimage (&img_continue_pick, ".\\pic\\continue_pick.jpg");

    loadimage (&img_options, ".\\pic\\options.jpg");
    loadimage (&img_options_cover, ".\\pic\\options_cover.jpg");
    loadimage (&img_options_pick, ".\\pic\\options_pick.jpg");
    loadimage (&img_volume, ".\\pic\\volume.jpg");
    loadimage (&img_volume_control, ".\\pic\\volume_control.jpg");
    loadimage (&img_volume_button, ".\\pic\\volume_button.jpg");
    loadimage (&img_volume_button_cover, ".\\pic\\volume_button_cover.jpg");
    loadimage (&img_volume_button_pick, ".\\pic\\volume_button_pick.jpg");
    loadimage (&img_volume_button_pick_cover, ".\\pic\\volume_button_pick_cover.jpg");
    loadimage (&img_sound_button, ".\\pic\\sound_button.jpg");
    loadimage (&img_sound_button_cover, ".\\pic\\sound_button_cover.jpg");
    loadimage (&img_sound_button_pick, ".\\pic\\sound_button_pick.jpg");
    loadimage (&img_sound_button_pick_cover, ".\\pic\\sound_button_pick_cover.jpg");
    loadimage (&img_back, ".\\pic\\back.jpg");
    loadimage (&img_back_cover, ".\\pic\\back_cover.jpg");
    loadimage (&img_back_pick, ".\\pic\\back_pick.jpg");
    loadimage (&img_back_pick_cover, ".\\pic\\back_pick_cover.jpg");

    loadimage (&img_pause_background, ".\\pic\\pause_background.jpg");

    loadimage (&img_loading, ".\\pic\\loading.jpg");
    loadimage (&img_loading_bar, ".\\pic\\loading_bar.jpg");
    loadimage (&img_little_loading, ".\\pic\\little_loading.jpg");
    loadimage (&img_little_loading_bk, ".\\pic\\little_loading_bk.jpg");
    loadimage (&img_little_loading2, ".\\pic\\little_loading.jpg");

    loadimage (&img_player, ".\\pic\\player.jpg");
    loadimage (&img_player_cover, ".\\pic\\player_cover.jpg");
    loadimage (&img_hp, ".\\pic\\hp.jpg");
    loadimage (&img_hp_cover, ".\\pic\\hp_cover.jpg");
    loadimage (&img_s, ".\\pic\\s.jpg");
    loadimage (&img_w, ".\\pic\\w.jpg");
    loadimage (&img_a, ".\\pic\\a.jpg");
    loadimage (&img_d, ".\\pic\\d.jpg");
    loadimage (&img_s_cover, ".\\pic\\s_cover.jpg");
    loadimage (&img_w_cover, ".\\pic\\w_cover.jpg");
    loadimage (&img_a_cover, ".\\pic\\a_cover.jpg");
    loadimage (&img_d_cover, ".\\pic\\d_cover.jpg");

    img_enemy = img_player;         // 加载敌人图片
    img_enemy_cover = img_player_cover;
    img_enemy_hp_cover = img_hp_cover;
    img_enemy_a = img_a;
    img_enemy_s = img_s;
    img_enemy_d = img_d;
    img_enemy_w = img_w;
    img_enemy_a_cover = img_a_cover;
    img_enemy_s_cover = img_s_cover;
    img_enemy_d_cover = img_d_cover;
    img_enemy_w_cover = img_w_cover;


//	loadimage (&img_gun, ".\\pic\\gun_a.jpg");
    loadimage (&img_shotgun_a, ".\\pic\\shotgun_a.jpg");
    loadimage (&img_shotgun_a_cover, ".\\pic\\shotgun_a_cover.jpg");
    loadimage (&img_shotgun_d, ".\\pic\\shotgun_d.jpg");
    loadimage (&img_shotgun_d_cover, ".\\pic\\shotgun_d_cover.jpg");
    loadimage (&img_shotgun_s, ".\\pic\\shotgun_s.jpg");
    loadimage (&img_shotgun_s_cover, ".\\pic\\shotgun_s_cover.jpg");
    loadimage (&img_shotgun_w, ".\\pic\\shotgun_w.jpg");
    loadimage (&img_shotgun_w_cover, ".\\pic\\shotgun_w_cover.jpg");
    loadimage (&img_uzi_a, ".\\pic\\uzi_a.jpg");
    loadimage (&img_uzi_a_cover, ".\\pic\\uzi_a_cover.jpg");
    loadimage (&img_uzi_d, ".\\pic\\uzi_d.jpg");
    loadimage (&img_uzi_d_cover, ".\\pic\\uzi_d_cover.jpg");
    loadimage (&img_uzi_s, ".\\pic\\uzi_s.jpg");
    loadimage (&img_uzi_s_cover, ".\\pic\\uzi_s_cover.jpg");
    loadimage (&img_uzi_w, ".\\pic\\uzi_w.jpg");
    loadimage (&img_uzi_w_cover, ".\\pic\\uzi_w_cover.jpg");
    loadimage (&img_handgun_a, ".\\pic\\handgun_a.jpg");
    loadimage (&img_handgun_a_cover, ".\\pic\\handgun_a_cover.jpg");
    loadimage (&img_handgun_d, ".\\pic\\handgun_d.jpg");
    loadimage (&img_handgun_d_cover, ".\\pic\\handgun_d_cover.jpg");
    loadimage (&img_handgun_s, ".\\pic\\handgun_s.jpg");
    loadimage (&img_handgun_s_cover, ".\\pic\\handgun_s_cover.jpg");
    loadimage (&img_handgun_w, ".\\pic\\handgun_w.jpg");
    loadimage (&img_handgun_w_cover, ".\\pic\\handgun_w_cover.jpg");

    img_gun_a = img_handgun_a;
    img_gun_a_cover = img_handgun_a_cover;
    img_gun_d = img_handgun_d;
    img_gun_d_cover = img_handgun_d_cover;
    img_gun_s = img_handgun_s;
    img_gun_s_cover = img_handgun_s_cover;
    img_gun_w = img_handgun_w;
    img_gun_w_cover = img_handgun_w_cover;

    loadimage (&img_dazhao, ".\\pic\\dazhao.jpg");

    loadimage (&img_zombie_a, ".\\pic\\zombie_a.jpg");
    loadimage (&img_zombie_a_cover, ".\\pic\\zombie_a_cover.jpg");
    loadimage (&img_zombie_w, ".\\pic\\zombie_w.jpg");
    loadimage (&img_zombie_w_cover, ".\\pic\\zombie_w_cover.jpg");
    loadimage (&img_zombie_s, ".\\pic\\zombie_s.jpg");
    loadimage (&img_zombie_s_cover, ".\\pic\\zombie_s_cover.jpg");
    loadimage (&img_zombie_d, ".\\pic\\zombie_d.jpg");
    loadimage (&img_zombie_d_cover, ".\\pic\\zombie_d_cover.jpg");
    loadimage (&img_zombie_aw, ".\\pic\\zombie_aw.jpg");
    loadimage (&img_zombie_aw_cover, ".\\pic\\zombie_aw_cover.jpg");
    loadimage (&img_zombie_dw, ".\\pic\\zombie_dw.jpg");
    loadimage (&img_zombie_dw_cover, ".\\pic\\zombie_dw_cover.jpg");
    loadimage (&img_zombie_as, ".\\pic\\zombie_as.jpg");
    loadimage (&img_zombie_as_cover, ".\\pic\\zombie_as_cover.jpg");
    loadimage (&img_zombie_ds, ".\\pic\\zombie_ds.jpg");
    loadimage (&img_zombie_ds_cover, ".\\pic\\zombie_ds_cover.jpg");
    loadimage (&img_zombie_blood, ".\\pic\\zombie_blood.jpg");
    loadimage (&img_zombie_blood_cover, ".\\pic\\zombie_blood_cover.jpg");

    loadimage (&img_redbox, ".\\pic\\redbox.jpg");
    loadimage (&img_redbox_cover, ".\\pic\\redbox_cover.jpg");

    loadimage (&img_gameover, ".\\pic\\gameover.jpg");

    loadimage (&img_0, ".\\pic\\0.jpg");
    loadimage (&img_0_cover, ".\\pic\\0_cover.jpg");
    loadimage (&img_1, ".\\pic\\1.jpg");
    loadimage (&img_1_cover, ".\\pic\\1_cover.jpg");
    loadimage (&img_2, ".\\pic\\2.jpg");
    loadimage (&img_2_cover, ".\\pic\\2_cover.jpg");
    loadimage (&img_3, ".\\pic\\3.jpg");
    loadimage (&img_3_cover, ".\\pic\\3_cover.jpg");
    loadimage (&img_4, ".\\pic\\4.jpg");
    loadimage (&img_4_cover, ".\\pic\\4_cover.jpg");
    loadimage (&img_5, ".\\pic\\5.jpg");
    loadimage (&img_5_cover, ".\\pic\\5_cover.jpg");
    loadimage (&img_6, ".\\pic\\6.jpg");
    loadimage (&img_6_cover, ".\\pic\\6_cover.jpg");
    loadimage (&img_7, ".\\pic\\7.jpg");
    loadimage (&img_7_cover, ".\\pic\\7_cover.jpg");
    loadimage (&img_8, ".\\pic\\8.jpg");
    loadimage (&img_8_cover, ".\\pic\\8_cover.jpg");
    loadimage (&img_9, ".\\pic\\9.jpg");
    loadimage (&img_9_cover, ".\\pic\\9_cover.jpg");

    loadimage (&img_s0, ".\\pic\\s0.jpg");
    loadimage (&img_s0_cover, ".\\pic\\s0_cover.jpg");
    loadimage (&img_s1, ".\\pic\\s1.jpg");
    loadimage (&img_s1_cover, ".\\pic\\s1_cover.jpg");
    loadimage (&img_s2, ".\\pic\\s2.jpg");
    loadimage (&img_s2_cover, ".\\pic\\s2_cover.jpg");
    loadimage (&img_s3, ".\\pic\\s3.jpg");
    loadimage (&img_s3_cover, ".\\pic\\s3_cover.jpg");
    loadimage (&img_s4, ".\\pic\\s4.jpg");
    loadimage (&img_s4_cover, ".\\pic\\s4_cover.jpg");
    loadimage (&img_s5, ".\\pic\\s5.jpg");
    loadimage (&img_s5_cover, ".\\pic\\s5_cover.jpg");
    loadimage (&img_s6, ".\\pic\\s6.jpg");
    loadimage (&img_s6_cover, ".\\pic\\s6_cover.jpg");
    loadimage (&img_s7, ".\\pic\\s7.jpg");
    loadimage (&img_s7_cover, ".\\pic\\s7_cover.jpg");
    loadimage (&img_s8, ".\\pic\\s8.jpg");
    loadimage (&img_s8_cover, ".\\pic\\s8_cover.jpg");
    loadimage (&img_s9, ".\\pic\\s9.jpg");
    loadimage (&img_s9_cover, ".\\pic\\s9_cover.jpg");

    loadimage (&img_snum[0], ".\\pic\\s0.jpg");
    loadimage (&img_snum_cover[0], ".\\pic\\s0_cover.jpg");
    loadimage (&img_snum[1], ".\\pic\\s0.jpg");
    loadimage (&img_snum_cover[1], ".\\pic\\s0_cover.jpg");
    loadimage (&img_snum[2], ".\\pic\\s0.jpg");
    loadimage (&img_snum_cover[2], ".\\pic\\s0_cover.jpg");
    loadimage (&img_snum[3], ".\\pic\\s0.jpg");
    loadimage (&img_snum_cover[3], ".\\pic\\s0_cover.jpg");
    loadimage (&img_snum[4], ".\\pic\\s0.jpg");
    loadimage (&img_snum_cover[4], ".\\pic\\s0_cover.jpg");
    loadimage (&img_snum[5], ".\\pic\\s0.jpg");
    loadimage (&img_snum_cover[5], ".\\pic\\s0_cover.jpg");
    loadimage (&img_snum[6], ".\\pic\\s0.jpg");
    loadimage (&img_snum_cover[6], ".\\pic\\s0_cover.jpg");
    loadimage (&img_snum[7], ".\\pic\\s0.jpg");
    loadimage (&img_snum_cover[7], ".\\pic\\s0_cover.jpg");
    loadimage (&img_snum[8], ".\\pic\\s0.jpg");
    loadimage (&img_snum_cover[8], ".\\pic\\s0_cover.jpg");
    loadimage (&img_snum[9], ".\\pic\\s0.jpg");
    loadimage (&img_snum_cover[9], ".\\pic\\s0_cover.jpg");

    loadimage (&img_combo, ".\\pic\\combo.jpg");
    loadimage (&img_combo_cover, ".\\pic\\combo_cover.jpg");
    loadimage (&img_combo_x, ".\\pic\\combo_x.jpg");
    loadimage (&img_combo_x_cover, ".\\pic\\combo_x_cover.jpg");
    loadimage (&img_combo_p, ".\\pic\\combo_p.jpg");
    loadimage (&img_combo_p_cover, ".\\pic\\combo_p_cover.jpg");

    loadimage (&img_rank_pic, ".\\pic\\rank_pic.jpg");
}

void startup()
{
    //初始化
    // 获得窗口句柄
    HWND hWnd = GetHWnd();
    // 使用 API 函数修改窗口名称
    SetWindowText(hWnd, "僵尸危机");       //目前未调试成功
    srand((unsigned)time(NULL));
    int i;
    start = 0;
    pvp = 0;
    initgraph(Width,High);
    load_img();

    map_x = 0;
    map_y = 0;
    wall_x_a = 0;
    wall_x_b = 0;
    wall_y_a = 0;
    wall_y_b = 0;

    pick_start = 0;
    pick_tips = 0;
    pick_options = 0;
    pick_pvp = 0;
    pick_continue = 0;
    pick_rank = 0;
    pick_quit = 0;
    pick_join = 0;
    pick_create = 0;
    choose_pvp = 0;
    pick_backtomenu = 0;

    bkmusic = 1;         //音乐开关
    sound_switch = 1;    // 音效开关
    volume = 300;
    ctrl_volume = 0;          // 是否处于控制状态
    volume_x = 300;       // 音量条位置
    volume_control_x = volume_x + 358;      //音量条控制键位置
    sound = 300;
    ctrl_sound = 0;
    sound_x = 300;
    sound_control_x = sound_x + 358;

    outmap_x = 0;
    outmap_y = 0;

    player_x = 0;
    player_y = 0;  // 在中心基础上的人物移动
    player_walk = 0;
    player_hp = 100;

    dazhao = 0;
    dazhao_xuli = 0;
    dazhao_need = 10;      // 需要杀死20个zombie
    dazhao_time = 0;
    dazhao_time_limit = 3000;    // 大招时长3000次循环

    combo = 1;     //初始连击为1
    combo_time = 0;    // 连击时间为0
    combo_time_limit = 1000;  // 连击有效时间1000次循环
    level = 1;    // 初始1级
    score = 0;    // 分数
    fire = 0;   // 开火
    gun_change = 0;     // 不在换枪状态

    img_gun = img_gun_a;    //初始化gun状态
    img_gun_cover = img_gun_a_cover;
    gun_model = 1;           // 1为handgun
    gun_level = 3;           // 测试阶段直接解锁两把枪
    hit = 0;
    initialize_shotgun();
    initialize_uzi();
    initialize_handgun();
    initialize_enemy();
    gun_present = handgun;

    player_direction = 's';   // 默认方向向下

    for (i = 0; i < redbox_num; i ++){
        rb[i].exist = 1;
        rb[i].pick = 0;
        rb[i].x = 1600 / 5 * (i + 1);
        rb[i].y = 600 / 2;
    }
    for (i = redbox_num; i < redbox_num_limit; i ++){
        rb[i].exist = 0;
    }

    pause_exit = 0;
    int p;   // 用于随机僵尸出生位置
    zombie_num = 10;     // 最开始是10个僵尸
    for (i = 0; i < zombie_num; i ++){
        p = rand() % 2;    // 两个方向出现僵尸
        if (p == 0){
            img_zombie[i] = img_zombie_d;     // 初始化僵尸方向
            img_zombie_cover[i] = img_zombie_d_cover;
            zombie_move_direction[i] = 'd';
            zombie_x[i] = -300;
            zombie_y[i] = rand() % 450 + 150;   // 初始化僵尸位置
        }
        else if (p == 1){
            img_zombie[i] = img_zombie_a;     // 初始化僵尸方向
            img_zombie_cover[i] = img_zombie_a_cover;
            zombie_move_direction[i] = 'a';
            zombie_x[i] = 1900;
            zombie_y[i] = rand() % 450 + 150;   // 初始化僵尸位置
        }
        attack[i] = 0;
        zombie_move_count[i] = 0;
        zombie_attack_count[i] = 0;
        zombie_hp[i] = 10;       // 初始化僵尸血量
        zombie_alive[i] = 1;    // 初始化僵尸存活
        zombie_walk[i] = 0;
        zombie_walk_count[i] = 0;
    }
    for (i = zombie_num; i < zombie_num_limit; i ++){
        zombie_alive[i] = 0;               // 定义为死僵尸
    }
    setbkmode(TRANSPARENT);
    settextcolor(YELLOW);  //更改字体样式
    BeginBatchDraw();
}



/*=============================================菜单界面==================================================*/
/*=============================================菜单界面==================================================*/
void menu_show(){
    int a;
    a = rand() % 5;
    int b;
    b = rand() % 5;
    putimage (0, 0, &img_menu_background);
    if (pick_start == 1){
        if (sound_switch == 1){
            mciSendString("open bm_music\\pass_button.mp3 alias pass", NULL, 0, NULL);//鼠标经过按键的声音
            mciSendString("play pass", NULL, 0, NULL);
        }
        putimage (420 + a, 105 + b, &img_singleplay_cover,NOTSRCERASE);               //特效
        putimage (420 + a, 105 + b, &img_singleplay_pick,SRCINVERT);
    }
    else{
        putimage (420, 105, &img_singleplay_cover,NOTSRCERASE);
        putimage (420, 105, &img_singleplay,SRCINVERT);
    }

    if (pick_tips == 1){       // 提示菜单
        if (sound_switch == 1){
            mciSendString("open bm_music\\pass_button.mp3 alias pass", NULL, 0, NULL);//鼠标经过按键的声音
            mciSendString("play pass", NULL, 0, NULL);
        }
        putimage (420 + 125 + a, 175 + b, &img_tips_cover,NOTSRCERASE);               //特效
        putimage (420 + 125 + a, 175 + b, &img_tips_pick,SRCINVERT);
    }
    else{
        putimage (420 + 125, 175, &img_tips_cover,NOTSRCERASE);
        putimage (420 + 125, 175, &img_tips,SRCINVERT);
    }

    if (pick_options == 1){       // option菜单
        if (sound_switch == 1){
            mciSendString("open bm_music\\pass_button.mp3 alias pass", NULL, 0, NULL);//鼠标经过按键的声音
            mciSendString("play pass", NULL, 0, NULL);
        }
        putimage (420 + 60 + a, 245 + b, &img_options_cover,NOTSRCERASE);               //特效
        putimage (420 + 60 + a, 245 + b, &img_options_pick,SRCINVERT);
    }
    else{
        putimage (420 + 60, 245, &img_options_cover,NOTSRCERASE);
        putimage (420 + 60, 245, &img_options,SRCINVERT);
    }

    if (choose_pvp == 0){               // 未选中pvp
        if (pick_pvp == 1){       // pvp菜单
            if (sound_switch == 1){
                mciSendString("open bm_music\\pass_button.mp3 alias pass", NULL, 0, NULL);//鼠标经过按键的声音
                mciSendString("play pass", NULL, 0, NULL);
            }
            putimage (420 + 120 + a, 310 + b, &img_pvp_cover,NOTSRCERASE);               //特效
            putimage (420 + 120 + a, 310 + b, &img_pvp_pick,SRCINVERT);
        }
        else{
            putimage (420 + 120, 310, &img_pvp_cover,NOTSRCERASE);
            putimage (420 + 120, 310, &img_pvp,SRCINVERT);
        }
    }
    else if (choose_pvp == 1){            // 选中pvp
        if (pick_create == 1){       // pvpcreate菜单
            if (sound_switch == 1){
                mciSendString("open bm_music\\pass_button.mp3 alias pass", NULL, 0, NULL);//鼠标经过按键的声音
                mciSendString("play pass", NULL, 0, NULL);
            }
            putimage (420 - 30 + a, 310 + b, &img_create_cover,NOTSRCERASE);               //特效
            putimage (420 - 30 + a, 310 + b, &img_create_pick,SRCINVERT);
        }
        else{
            putimage (420 - 30, 310, &img_create_cover,NOTSRCERASE);
            putimage (420 - 30, 310, &img_create,SRCINVERT);
        }

        if (pick_join == 1){       // pvpjoin菜单
            if (sound_switch == 1){
                mciSendString("open bm_music\\pass_button.mp3 alias pass", NULL, 0, NULL);//鼠标经过按键的声音
                mciSendString("play pass", NULL, 0, NULL);
            }
            putimage (420 + 220 + a, 300 + b, &img_join_cover,NOTSRCERASE);               //特效
            putimage (420 + 220 + a, 300 + b, &img_join_pick,SRCINVERT);
        }
        else{
            putimage (420 + 220, 300, &img_join_cover,NOTSRCERASE);
            putimage (420 + 220, 300, &img_join,SRCINVERT);
        }
    }

    if (pick_rank == 1){       // rank菜单
        if (sound_switch == 1){
            mciSendString("open bm_music\\pass_button.mp3 alias pass", NULL, 0, NULL);//鼠标经过按键的声音
            mciSendString("play pass", NULL, 0, NULL);
        }
        putimage (420 + 100 + a, 375 + b, &img_rank_cover,NOTSRCERASE);               //特效
        putimage (420 + 100 + a, 375 + b, &img_rank_pick,SRCINVERT);
    }
    else{
        putimage (420 + 100, 375, &img_rank_cover,NOTSRCERASE);
        putimage (420 + 100, 375, &img_rank,SRCINVERT);
    }

    if (pick_quit == 1){       // quit菜单
        if (sound_switch == 1){
            mciSendString("open bm_music\\pass_button.mp3 alias pass", NULL, 0, NULL);//鼠标经过按键的声音
            mciSendString("play pass", NULL, 0, NULL);
        }
        putimage (420 + 110 + a, 440 + b, &img_quit_cover,NOTSRCERASE);               //特效
        putimage (420 + 110 + a, 440 + b, &img_quit_pick,SRCINVERT);
    }
    else{
        putimage (420 + 110, 440, &img_quit_cover,NOTSRCERASE);
        putimage (420 + 110, 440, &img_quit,SRCINVERT);
    }



    if (pick_options != 1 && pick_start != 1 && pick_tips != 1 && pick_pvp != 1
        && pick_rank != 1 && pick_quit != 1){
        mciSendString("close pass", NULL, 0, NULL);   // 关闭音效
    }

    FlushBatchDraw();
    Sleep(2);
}

void tips()    // 打开提示
{
    void little_loading();
    little_loading();
    putimage (0, 0, &img_tips_picture);
    FlushBatchDraw();
    Sleep(2);
    system ("pause");
    little_loading();
}

void menu_quit()   //退出游戏
{
    void little_loading();
    little_loading();
    EndBatchDraw();
    mciSendString("close bk", NULL, 0, NULL);
    closegraph();
    exit(0);// 游戏结束
}

void volume_update()    // 更新音量
{
    if (volume >= 250){                                   // 音量调节
        mciSendString("setaudio bk volume to 300", NULL, 0, NULL);   // 音量调节为300
    }
    else if (volume >= 200){
        mciSendString("setaudio bk volume to 250", NULL, 0, NULL);   // 音量调节为250
    }
    else if (volume >= 150){
        mciSendString("setaudio bk volume to 200", NULL, 0, NULL);   // 音量调节为200
    }
    else if (volume >= 100){
        mciSendString("setaudio bk volume to 150", NULL, 0, NULL);   // 音量调节为150
    }
    else if (volume >= 50){
        mciSendString("setaudio bk volume to 100", NULL, 0, NULL);   // 音量调节为100
    }
    else if (volume >= 20){
        mciSendString("setaudio bk volume to 50", NULL, 0, NULL);   // 音量调节为50
    }
    else{
        mciSendString("setaudio bk volume to 0", NULL, 0, NULL);   // 音量调节为0
    }

    if (bkmusic == -1){      // 关闭背景音乐
        mciSendString("pause bk", NULL, 0, NULL);
    }
    else if (bkmusic == 1){      // 恢复背景音乐
        mciSendString("resume bk", NULL, 0, NULL);
    }
}

void options()             // 选项菜单
{
    void little_loading();
    little_loading();
    int quit;
    int volume_pick;
    int sound_pick;
    int back_pick;
    volume_pick = 0;
    sound_pick = 0;
    back_pick = 0;
    quit = 0;

    while (1)
    {
        putimage (0, 0, &img_pause_background);

        putimage (volume_x, 200, &img_volume);
        putimage (sound_x, 290, &img_volume);
        putimage (volume_control_x, 193, &img_volume_control);
        putimage (sound_control_x, 283, &img_volume_control);
        while (MouseHit())                  // 鼠标操作
        {
            m=GetMouseMsg();
            if (m.x >= volume_control_x && m.x <= volume_control_x + 35
                && m.y >= 193 && m.y <= 193 + 40){
                if (m.uMsg == WM_LBUTTONDOWN)           // 鼠标调节音量
                {
                    ctrl_volume = 1;
                }
            }

            if (m.x >= sound_control_x && m.x <= sound_control_x + 35
                && m.y >= 283 && m.y <= 283 + 40){
                if (m.uMsg == WM_LBUTTONDOWN)           // 鼠标调节音量
                {
                    ctrl_sound = 1;
                }
            }

            if (m.uMsg == WM_LBUTTONUP)           // 鼠标调节音量
            {
                ctrl_volume = 0;
                ctrl_sound = 0;
            }

            if (m.x >= volume_x - 120 && m.x <= volume_x - 120 + 90 && m.y >= 190 && m.y <= 190 + 45)
            {
                volume_pick = 1;// 音量图标选中
                if (m.uMsg == WM_LBUTTONDOWN)           // 开关背景音乐
                {
                    bkmusic = -bkmusic;
                }
            }
            else{
                volume_pick = 0;
            }

            if (m.x >= volume_x - 120 && m.x <= volume_x - 120 + 90 && m.y >= 280 && m.y <= 280 + 45)
            {
                sound_pick = 1; // 音效图标选中
                if (m.uMsg == WM_LBUTTONDOWN)           // 开关音效
                {
                    sound_switch = -sound_switch;
                }
            }
            else{
                sound_pick = 0;
            }

            if (m.x >= 450 && m.x <= 450 + 83 && m.y >= 450 && m.y <= 450 + 36)
            {
                back_pick = 1;// 返回图标选中
                if (m.uMsg == WM_LBUTTONDOWN)           // 退出options
                {
                    quit = 1;
                }
            }
            else{
                back_pick = 0;
            }

        }

        if (volume_pick == 1)
        {
            putimage (volume_x - 120, 190, &img_volume_button_pick_cover, NOTSRCERASE);    // 放置音量图标选中
            putimage (volume_x - 120, 190, &img_volume_button_pick, SRCINVERT);
        }
        else{
            putimage (volume_x - 120, 190, &img_volume_button_cover, NOTSRCERASE);    // 放置音量图标
            putimage (volume_x - 120, 190, &img_volume_button, SRCINVERT);
        }

        if (sound_pick == 1)
        {
            putimage (volume_x - 120, 280, &img_sound_button_pick_cover, NOTSRCERASE);    // 放置音效图标选中
            putimage (volume_x - 120, 280, &img_sound_button_pick, SRCINVERT);
        }
        else{
            putimage (volume_x - 120, 280, &img_sound_button_cover, NOTSRCERASE);    // 放置音效图标
            putimage (volume_x - 120, 280, &img_sound_button, SRCINVERT);
        }

        if (back_pick == 1)
        {
            putimage (450, 450, &img_back_pick_cover, NOTSRCERASE);    // 放置返回图标选中
            putimage (450, 450, &img_back_pick, SRCINVERT);
        }
        else{
            putimage (450, 450, &img_back_cover, NOTSRCERASE);    // 放置返回图标
            putimage (450, 450, &img_back, SRCINVERT);
        }

        if (ctrl_volume == 1){
            volume_control_x = m.x - 17;        // 音量条滑动
        }
        if (ctrl_sound == 1){
            sound_control_x = m.x - 17;        // 音量条滑动
        }

        if (volume_control_x >= 700 - 42){                     // 防止控制块滑出去
            volume_control_x = 700 - 42;
        }
        else if (volume_control_x <= 300 + 7){             // 防止控制块滑出去
            volume_control_x = 300 + 7;
        }
        if (sound_control_x >= 700 - 42){                     // 防止控制块滑出去
            sound_control_x = 700 - 42;
        }
        else if (sound_control_x <= 300 + 7){             // 防止控制块滑出去
            sound_control_x = 300 + 7;
        }

        volume = 1.0 * (volume_control_x - volume_x) / 390 * 300;         // 更新音量
        sound = 1.0 * (sound_control_x - sound_x) / 390 * 300;         // 更新音效

        volume_update();

        if (quit == 1){             // 退出options
            break;
        }

        FlushBatchDraw();
        Sleep(2);
    }
    little_loading();

}

void rank()    // 显示最高分
{
    FILE *fp;
    int highest;
    char c[10];
    fp = fopen(".\\rank.zsc","r");
    fscanf(fp,"最高分：%d", &highest);
    fclose(fp);
    sprintf(c, "%d", highest);
    putimage(200, 150, &img_rank_pic);
    settextstyle(30, 0, _T("宋体"));
    outtextxy(Width / 2 - 110, High / 2, "最高分：");
    outtextxy(Width / 2 , High / 2, c);
    FlushBatchDraw();
    settextstyle(15, 0, _T("宋体"));
    Sleep(1500);
}

void pvp_choose()
{
    if (m.uMsg == WM_LBUTTONDOWN)     //服务器模式
    {
        pthread_t t1;     // 开启多线程
        pthread_create(&t1,NULL,tprocess1,NULL);
        start = 1;
        pvp = 1;
    }

    if (m.uMsg == WM_RBUTTONDOWN)     //客户端模式
    {
        pthread_t t2;     // 开启多线程
        pthread_create(&t2,NULL,tprocess2,NULL);
        start = 1;
        pvp = 1;
    }
}

void menu_user(){
    while (MouseHit())
    {
        m=GetMouseMsg();
        if (m.x >= 420 && m.x <= 420 + 333 && m.y >= 105 && m.y <= 105 + 39)   // 点击single
        {
            pick_start = 1;
            if (m.uMsg == WM_LBUTTONDOWN)
            {
                start = 1;
            }
        }
        else{
            pick_start = 0;
        }

        if (m.x >= 420 + 125 && m.x <= 420 + 125 + 76 && m.y >= 175 && m.y <= 175 + 39)          // 点击tips
        {
            pick_tips = 1;
            if (m.uMsg == WM_LBUTTONDOWN)
            {
                tips();     // 打开提示
            }
        }
        else{
            pick_tips = 0;
        }

        if (m.x >= 420 + 60&& m.x <= 420 + 60 + 229 && m.y >= 245 && m.y <= 245 + 39)    // 点击options
        {
            pick_options = 1;
            if (m.uMsg == WM_LBUTTONDOWN)
            {
                options();   //  打开设置菜单
            }
        }
        else{
            pick_options = 0;
        }

        if (choose_pvp == 0 && m.x >= 420 + 120 && m.x <= 420 + 120 + 96 && m.y >= 310 && m.y <= 310 + 39)    // 点击pvp
        {
            pick_pvp = 1;
            if (m.uMsg == WM_LBUTTONDOWN)
            {
                choose_pvp = 1;
            }
        }
        else{
            pick_pvp = 0;
        }

        if (choose_pvp == 1 && m.x >= 420 - 30 && m.x <= 420 - 30 + 163 && m.y >= 310 && m.y <= 310 + 39)   // 选择创建房间
        {
            pick_create = 1;
            if (m.uMsg == WM_LBUTTONDOWN)
            {
                pthread_t t1;     // 开启多线程
                pthread_create(&t1,NULL,tprocess1,NULL);
                start = 1;
                pvp = 1;
            }
        }
        else{
            pick_create = 0;
        }

        if (choose_pvp == 1 && m.x >= 420 + 220 && m.x <= 420 + 220 + 100 && m.y >= 300 && m.y <= 300 + 55)   // 选择加入房间
        {
            pick_join = 1;
            if (m.uMsg == WM_LBUTTONDOWN)
            {
                pthread_t t2;     // 开启多线程
                pthread_create(&t2,NULL,tprocess2,NULL);
                start = 1;
                pvp = 1;
            }
        }
        else{
            pick_join = 0;
        }

        if (choose_pvp == 1 && (m.x < 420 - 30 || m.x > 420 + 220 + 100 || m.y > 300 + 55 || m.y < 300)){
            choose_pvp = 0;
        }

        if (m.x >= 420 + 100 && m.x <= 420 + 100 + 144 && m.y >= 375 && m.y <= 375 + 39)    // 点击rank
        {
            pick_rank= 1;
            if (m.uMsg == WM_LBUTTONDOWN)
            {
                rank();   //  打开rank
            }

        }
        else{
            pick_rank = 0;
        }

        if (m.x >= 420 + 110 && m.x <= 420 + 110 + 120 && m.y >= 440 && m.y <= 440 + 39)    // 点击quit
        {
            pick_quit= 1;
            if (m.uMsg == WM_LBUTTONDOWN)
            {
                menu_quit();   //  退出游戏
            }

        }
        else{
            pick_quit = 0;
        }

    }
}

void little_loading()  //小加载
{
    int i;
    int angle;
    angle = 0;
    putimage(0, 0, &img_little_loading_bk);
    for (i = 0; i < 50; i ++){
        angle = angle + 1;
        rotateimage(&img_little_loading2, &img_little_loading, PI / 100 * angle);
        putimage(Width / 2 - 35, High / 2 - 35, &img_little_loading2);
        FlushBatchDraw();
        Sleep (10);
    }

}

void loading()     // 加载进度条   110*281
{
    int load;
    load = 0;
    putimage (0, 0, &img_loading);
    for (load = 0; load < 28; load ++){
        putimage(110 + load * 20, 281, &img_loading_bar);
        Sleep(50);
        FlushBatchDraw();
    }
    Sleep (600);
}

void menu(){                 // 开始界面菜单
    mciSendString("open bm_music\\menumusic.mp3 alias bk", NULL, 0, NULL);//打开背景音乐
    mciSendString("play bk repeat", NULL, 0, NULL);  // 循环播放
    mciSendString("setaudio bk volume to 300", NULL, 0, NULL);   // 音量调节为300 大概是最大值了
    while(start == 0){
        menu_user();
        menu_show();
    }
    if (sound_switch == 1){
        mciSendString("open bm_music\\enter.mp3 alias enter", NULL, 0, NULL);//进入游戏音效
        mciSendString("play enter", NULL, 0, NULL);
    }
    loading();
    mciSendString("close enter", NULL, 0, NULL);
    mciSendString("close bk", NULL, 0, NULL);
    if (pvp == 0){
        mciSendString("open bm_music\\music.mp3 alias bk", NULL, 0, NULL);//打开背景音乐
        mciSendString("play bk repeat", NULL, 0, NULL);  // 循环播放
    }
    else if (pvp == 1){
        mciSendString("open bm_music\\pvp.mp3 alias bk", NULL, 0, NULL);//打开背景音乐
        mciSendString("play bk repeat", NULL, 0, NULL);  // 循环播放
    }
    volume_update();

}
/*============================================暂停界面=====================================================*/
/*============================================暂停界面=================================================*/
void pause_show()
{
    int a;
    a = rand() % 5;
    int b;
    b = rand() % 5;
    putimage (0, 0, &img_pause_background);

    if (pick_tips == 1){       // 提示菜单
        if (sound_switch == 1){
            mciSendString("open bm_music\\pass_button.mp3 alias pass", NULL, 0, NULL);//鼠标经过按键的声音
            mciSendString("play pass", NULL, 0, NULL);
        }
        putimage (Width / 2 - 40 + a, 220 + b, &img_tips_cover,NOTSRCERASE);               //特效
        putimage (Width / 2 - 40 + a, 220 + b, &img_tips_pick,SRCINVERT);
    }
    else{
        putimage (Width / 2 - 40, 220, &img_tips_cover,NOTSRCERASE);
        putimage (Width / 2 - 40, 220, &img_tips,SRCINVERT);
    }

    if (pick_continue == 1){
        if (sound_switch == 1){
            mciSendString("open bm_music\\pass_button.mp3 alias pass", NULL, 0, NULL);//鼠标经过按键的声音
            mciSendString("play pass", NULL, 0, NULL);
        }
        putimage (252 + a, 150 + b, &img_continue_cover,NOTSRCERASE);               //特效
        putimage (252 + a, 150 + b, &img_continue_pick,SRCINVERT);
    }
    else{
        putimage (252, 150, &img_continue_cover,NOTSRCERASE);
        putimage (252, 150, &img_continue,SRCINVERT);
    }

    if (pick_backtomenu == 1){
        if (sound_switch == 1){
            mciSendString("open bm_music\\pass_button.mp3 alias pass", NULL, 0, NULL);//鼠标经过按键的声音
            mciSendString("play pass", NULL, 0, NULL);
        }
        putimage (207 + a, 290 + b, &img_backtomenu_cover,NOTSRCERASE);               //特效
        putimage (207 + a, 290 + b, &img_backtomenu_pick,SRCINVERT);
    }
    else{
        putimage (207, 290, &img_backtomenu_cover,NOTSRCERASE);
        putimage (207, 290, &img_backtomenu,SRCINVERT);
    }

    FlushBatchDraw();
    Sleep(2);
}

void pause_input()
{
    void gameover_back_to_menu();
    while (MouseHit())
    {
        m=GetMouseMsg();
        if (m.x >= Width / 2 - 40 && m.x <= Width / 2 - 40 + 76 && m.y >= 220 && m.y <= 220 + 39)          // 点击tips
        {
            pick_tips = 1;
            if (m.uMsg == WM_LBUTTONDOWN)
            {
                tips();     // 打开提示
            }
        }
        else{
            pick_tips = 0;
        }

        if (m.x >= 252 && m.x <= 252 + 333 && m.y >= 150 && m.y <= 150 + 39)   // 点击continue
        {
            pick_continue = 1;
            if (m.uMsg == WM_LBUTTONDOWN)
            {
                pause_exit = 1;
            }
        }
        else{
            pick_continue = 0;
        }

        if (m.x >= 207 && m.x <= 207 + 410 && m.y >= 290 && m.y <= 290 + 39)   // 点击backtomenu
        {
            pick_backtomenu = 1;
            if (m.uMsg == WM_LBUTTONDOWN)
            {
                little_loading();
                gameover_back_to_menu();
            }
        }
        else{
            pick_backtomenu = 0;
        }
    }

}

void pause()     // 暂停模块
{
    while (pause_exit == 0){
        pause_show();
        pause_input();
    }
    pause_exit = 0;
}

/*=========================================用户输入操作==============================================*/
/*=========================================用户输入操作==============================================*/


void user()//用户输入******************************
{
    if (outmap_x == 0){                       //移动
        if ((GetAsyncKeyState(0x41) & 0x8000)){ //a
            map_x++;
            player_direction = 'a';
            player_walk++;
        }
        if ((GetAsyncKeyState(0x44) & 0x8000)){ //d
            map_x--;
            player_direction = 'd';
            player_walk++;
        }

    }
    else if (outmap_x == 1){
        if ((GetAsyncKeyState(0x41) & 0x8000)){ //a
            player_x--;
            player_direction = 'a';
            player_walk++;
        }

        if ((GetAsyncKeyState(0x44) & 0x8000)){ //d
            player_x++;
            player_direction = 'd';
            player_walk++;
        }
        if (player_x == 0)
            outmap_x = 0;        //地图控制
    }

    if (outmap_y == 0){
        if (GetAsyncKeyState(0x57) & 0x8000 ){  // w
            map_y++;
            player_direction = 'w';
            player_walk++;
        }
        if ((GetAsyncKeyState(0x53) & 0x8000)){ //s
            map_y--;
            player_direction = 's';
            player_walk++;
        }
    }
    else if (outmap_y == 1){
        if (GetAsyncKeyState(0x57) & 0x8000 ){  // w
            player_y--;
            player_direction = 'w';
            player_walk++;
        }
        if ((GetAsyncKeyState(0x53) & 0x8000)){ //s
            player_y++;
            player_direction = 's';
            player_walk++;
        }
        if (player_y == 0)
            outmap_y = 0;         //地图控制
    }

    if ((GetAsyncKeyState(0x4A) & 0x8000)){   // 按j开火
        if (gun_present.gun_stop >= gun_present.gun_speed && gun_present.bullet > 0){
            fire = 1;
            gun_present.bullet --;
            hit = 0;
        }
    }

    if ((GetAsyncKeyState(0x31) & 0x8000)){   // 按1换枪
        gun_model_old = gun_model;   // 记录前一把枪
        gun_model = 1;
        gun_change = 1;
    }

    if ((GetAsyncKeyState(0x32) & 0x8000)){   // 按2换枪
        gun_model_old = gun_model;
        gun_model = 2;
        gun_change = 1;
    }

    if ((GetAsyncKeyState(0x33) & 0x8000)){   // 按3换枪
        gun_model_old = gun_model;
        gun_model = 3;
        gun_change = 1;
    }

    if ((GetAsyncKeyState(0x34) & 0x8000)){   // 按4换枪
        gun_model_old = gun_model;
        gun_model = 4;
        gun_change = 1;
    }

    if ((GetAsyncKeyState(0x50) & 0x8000)){   // 按p进入暂停界面
        pause();
    }

    if ((GetAsyncKeyState(0x4B) & 0x8000)){   // 按k释放大招
        if (dazhao_xuli >= dazhao_need){
            dazhao = 1;
            dazhao_time = dazhao_time_limit;
            gun_present.gun_speed = gun_present.gun_speed / 3;     // 开启大招增加射速
            mciSendString("open bm_music\\dazhao.mp3 alias dazhao", NULL, 0, NULL);//枪声
            mciSendString("play dazhao", NULL, 0, NULL);
        }
    }
/*
		char input;      //用于换枪
		if(kbhit())
		{
			input = getch();
			if (input == 'e'){
				gun_model_old = gun_model;
				gun_model ++;
				gun_change = 1;
			}
			if (input == 'q'){
				gun_model_old = gun_model;   // 记录前一把枪
				gun_model --;
				gun_change = 1;
			}
		}*/

}


/*============================================游戏自动更新===============================================*/
/*============================================游戏自动更新===============================================*/
void change_gun_img()  // 枪图片替换
{
    if (gun_model == 3){   // 加载shotgun图片
        img_gun_a = img_shotgun_a;
        img_gun_a_cover = img_shotgun_a_cover;
        img_gun_d = img_shotgun_d;
        img_gun_d_cover = img_shotgun_d_cover;
        img_gun_s = img_shotgun_s;
        img_gun_s_cover = img_shotgun_s_cover;
        img_gun_w = img_shotgun_w;
        img_gun_w_cover = img_shotgun_w_cover;
    }
    else if (gun_model == 2){ // 加载uzi
        img_gun_a = img_uzi_a;
        img_gun_a_cover = img_uzi_a_cover;
        img_gun_d = img_uzi_d;
        img_gun_d_cover = img_uzi_d_cover;
        img_gun_s = img_uzi_s;
        img_gun_s_cover = img_uzi_s_cover;
        img_gun_w = img_uzi_w;
        img_gun_w_cover = img_uzi_w_cover;
    }
    else if (gun_model == 1){ // 加载handgun
        img_gun_a = img_handgun_a;
        img_gun_a_cover = img_handgun_a_cover;
        img_gun_d = img_handgun_d;
        img_gun_d_cover = img_handgun_d_cover;
        img_gun_s = img_handgun_s;
        img_gun_s_cover = img_handgun_s_cover;
        img_gun_w = img_handgun_w;
        img_gun_w_cover = img_handgun_w_cover;
    }

}

void change_gun_info()
{
    if (gun_model_old == 1){      // 存储之前数据 子弹
        handgun.bullet = gun_present.bullet;
    }
    else if (gun_model_old == 2){
        uzi.bullet = gun_present.bullet;
    }
    else if (gun_model_old == 3){
        shotgun.bullet = gun_present.bullet;
    }

    if (gun_model == 1){        // 换枪数据
        gun_present = handgun;
    }
    else if (gun_model == 2){
        gun_present = uzi;
    }
    else if (gun_model == 3){
        gun_present = shotgun;
    }
}
void change_gun()  //更换武器
{
    if (gun_change == 1){
        if (gun_model <= 0){                        //控制换枪
            gun_model = 1;
        }
        else if (gun_model > gun_level){
            gun_model = gun_level;
        }
        else{
            change_gun_img();
            change_gun_info();
        }
        gun_change = 0;
    }
}
/*--------------------------------------------------------------------------------------------------*/
void update_walk()
{
    if (player_direction == 'w'){
        img_player = img_w;
        img_player_cover = img_w_cover;
    }
    else if (player_direction == 'a'){
        img_player = img_a;
        img_player_cover = img_a_cover;
    }
    else if (player_direction == 's'){
        img_player = img_s;
        img_player_cover = img_s_cover;
    }
    else if (player_direction == 'd'){
        img_player = img_d;
        img_player_cover = img_d_cover;
    }
    if (player_walk >= 4){
        player_walk = player_walk % 4;
    }
}

void hold_map(){   // 防止飞出地图
    if (map_x > 0){
        map_x = 0;
        outmap_x = 1;
    }
    if (map_y > 0){
        map_y = 0;
        outmap_y = 1;
    }
    if (map_x < 800 - 1600){
        map_x = 800 - 1600;
        outmap_x = 1;
    }
    if (map_y < 600 - 600){
        map_y = 600 - 600;
        outmap_y = 1;
    }
}

void zombie_move()   // 僵尸移动
{
    int x, y;   // 计算僵尸和玩家的距离
    int z;   // 用于斜方向行进判断
    int i;    // 循环变量

    for (i = 0; i < zombie_num; i ++){
        if (attack[i] == 0 && zombie_alive[i] == 1){
            zombie_move_count[i] ++;
            if (zombie_move_count[i] == zombie_move_speed){
                zombie_walk_count[i] ++;
                if (zombie_walk_count[i] > 15){
                    zombie_walk[i] ++;   // 走路图像控制
                    zombie_walk_count[i] = 0;
                }
                if (zombie_walk[i] >= 4){
                    zombie_walk[i] = zombie_walk[i] % 4;
                }
                x = abs(player_x + Width / 2 - map_x - zombie_x[i]);
                y = abs(player_y + High / 2 - map_y - zombie_y[i]);
                z = abs(x - y);

                if (x == 0 && y == 0){
                    zombie_move_direction[i] = 'p';
                }
                else if (z <= 50){                                                // 斜方向追人
                    if (player_x + Width / 2 - map_x - zombie_x[i] > 0 && player_y + High / 2 - map_y - zombie_y[i] > 0){
                        zombie_move_direction[i] = 'c';
                        img_zombie[i] = img_zombie_ds;
                        img_zombie_cover[i] = img_zombie_ds_cover;
                    }
                    if (player_x + Width / 2 - map_x - zombie_x[i] > 0 && player_y + High / 2 - map_y - zombie_y[i] < 0){
                        zombie_move_direction[i] = 'e';
                        img_zombie[i] = img_zombie_dw;
                        img_zombie_cover[i] = img_zombie_dw_cover;
                    }
                    if (player_x + Width / 2 - map_x - zombie_x[i] < 0 && player_y + High / 2 - map_y - zombie_y[i] > 0){
                        zombie_move_direction[i] = 'z';
                        img_zombie[i] = img_zombie_as;
                        img_zombie_cover[i] = img_zombie_as_cover;
                    }
                    if (player_x + Width / 2 - map_x - zombie_x[i] < 0 && player_y + High / 2 - map_y - zombie_y[i] < 0){
                        zombie_move_direction[i] = 'q';
                        img_zombie[i] = img_zombie_aw;
                        img_zombie_cover[i] = img_zombie_aw_cover;
                    }
                }
                else if (x > y && player_x + Width / 2 - map_x - zombie_x[i] > 0){       //     调整僵尸朝向 追人
                    zombie_move_direction[i] = 'd';
                    img_zombie[i] = img_zombie_d;
                    img_zombie_cover[i] = img_zombie_d_cover;
                }
                else if (x > y && player_x + Width / 2 - map_x - zombie_x[i] < 0){
                    zombie_move_direction[i] = 'a';
                    img_zombie[i] = img_zombie_a;
                    img_zombie_cover[i] = img_zombie_a_cover;
                }
                else if (x < y && player_y + High / 2 - map_y - zombie_y[i] > 0){
                    zombie_move_direction[i] = 's';
                    img_zombie[i] = img_zombie_s;
                    img_zombie_cover[i] = img_zombie_s_cover;
                }
                else if (x < y && player_y + High / 2 - map_y - zombie_y[i] < 0){
                    zombie_move_direction[i] = 'w';
                    img_zombie[i] = img_zombie_w;
                    img_zombie_cover[i] = img_zombie_w_cover;
                }

                //以下为僵尸移动
                if (zombie_move_direction[i] == 'p'){        //停在原地

                }
                else if (zombie_move_direction[i] == 'w'){
                    zombie_y[i] --;
                }
                else if (zombie_move_direction[i] == 'a'){
                    zombie_x[i] --;
                }
                else if (zombie_move_direction[i] == 's'){
                    zombie_y[i] ++;
                }
                else if (zombie_move_direction[i] == 'd'){
                    zombie_x[i] ++;
                }
                else if (zombie_move_direction[i] == 'q'){
                    zombie_y[i] --;
                    zombie_x[i] --;
                }
                else if (zombie_move_direction[i] == 'e'){
                    zombie_y[i] --;
                    zombie_x[i] ++;
                }
                else if (zombie_move_direction[i] == 'z'){
                    zombie_y[i] ++;
                    zombie_x[i] --;
                }
                else if (zombie_move_direction[i] == 'c'){
                    zombie_x[i] ++;
                    zombie_y[i] ++;
                }
                zombie_move_count[i] = 0;    // 计数器归零
            }
        }
    }

}

void player_attacked(int i)   // 玩家受击
{
    player_hp -= 1;
/*	if (zombie_move_direction[i] == 'a'){
		map_x -= 100;
	}
	else if (zombie_move_direction[i] == 'd'){
		map_x += 100;
	}
	else if (zombie_move_direction[i] == 'w'){
		map_y += 100;
	}
	else if (zombie_move_direction[i] == 's'){
		map_y -= 100;
	}
	else if (zombie_move_direction[i] == 'q'){
		map_y -= 100;
		map_x -= 100;
	}
	else if (zombie_move_direction[i] == 'e'){
		map_y -= 100;
		map_x += 100;
	}
	else if (zombie_move_direction[i] == 'z'){
		map_y += 100;
		map_x -= 100;
	}
	else if (zombie_move_direction[i] == 'c'){
		map_y += 100;
		map_x += 100;
	}*/   // 未调试成功

}
void zombie_attack(){
    int x, y, distance;
    int i;
    for(i = 0; i < zombie_num; i ++){
        if (zombie_alive[i] == 1){
            x = abs(player_x + 35/*一半的图片尺寸*/+ Width / 2 - map_x - (zombie_x[i] + 36));
            y = abs(player_y + 35/*一半的图片尺寸*/+ High / 2 - map_y - (zombie_y[i] + 50));
            distance = sqrt(x * x + y * y);
            if (distance <= 50){
                attack[i] = 1;
                zombie_attack_count[i] ++;
                zombie_move_direction[i] = 'p';
                if (zombie_attack_count[i] == zombie_attack_speed){
                    zombie_attack_count[i] = 0;
                    player_attacked(i);
                }
            }
            else
                attack[i] = 0;
        }
    }
}

void zombie_birth()        // 控制僵尸出现
{
    int i;
    int p;
    for (i = 0; i < zombie_num; i ++){
        if (zombie_alive[i] == 0){           // 判断死僵尸让僵尸重新出现
            p = rand() % 2;    // 两个方向出现僵尸
            if (p == 0){
                img_zombie[i] = img_zombie_d;     // 初始化僵尸方向
                img_zombie_cover[i] = img_zombie_d_cover;
                zombie_move_direction[i] = 'd';
                zombie_x[i] = -300;
                zombie_y[i] = rand() % 450 + 150;   // 初始化僵尸位置
            }
            else if (p == 1){
                img_zombie[i] = img_zombie_a;     // 初始化僵尸方向
                img_zombie_cover[i] = img_zombie_a_cover;
                zombie_move_direction[i] = 'a';
                zombie_x[i] = 1900;
                zombie_y[i] = rand() % 450 + 150;   // 初始化僵尸位置
            }
            attack[i] = 0;
            zombie_move_count[i] = 0;
            zombie_attack_count[i] = 0;
            zombie_hp[i] = 10;       // 初始化僵尸血量
            zombie_alive[i] = 1;    // 初始化僵尸存活
            zombie_walk[i] = 0;
            zombie_walk_count[i] = 0;
        }
    }

}

/*-----------------------------------碰撞（墙壁， 僵尸，人）----------------------------------------------*/


void touch_zombie_zombie(){                 // 僵尸碰撞
    int i, j;
    for (i = 0; i < zombie_num; i ++){
        for (j = i + 1; j < zombie_num; j ++){
            if (zombie_alive[i] == 1){
                if (sqrt((zombie_x[i] - zombie_x[j]) * (zombie_x[i] - zombie_x[j]) +
                         (zombie_y[i] - zombie_y[j]) * (zombie_y[i] - zombie_y[j])) <= 60){    //僵尸之间直线距离小于60
                    if (abs(zombie_x[i] - zombie_x[j]) > abs(zombie_y[i] - zombie_y[j])){
                        if (zombie_y[i] >= zombie_y[j])
                            zombie_y[i] ++;
                        else if(zombie_y[i] < zombie_y[j])
                            zombie_y[i] --;
                    }
                    else{
                        if (zombie_x[i] >= zombie_x[j])
                            zombie_x[i] ++;
                        else if(zombie_x[i] < zombie_x[j])
                            zombie_x[i] --;
                    }

                }
            }
        }
    }
}

void touch_player_wall(){
    if (player_x + Width / 2 - map_x > wall_x_a && player_x + Width / 2 - map_x < wall_x_b){       //在墙的竖直范围内 判断上下方向的碰撞
        if (player_y + High / 2 - map_y + 40> wall_y_a && player_y + High / 2 - map_y < wall_y_a + 20)
            player_y = player_y - 1;
        if (player_y + High / 2 - map_y + 40< wall_y_b && player_y + High / 2 - map_y > wall_y_b - 20)
            player_y = player_y + 1;
    }

    if (player_x + Width / 2 - map_x  <= 0 || player_x + Width / 2 - map_x  >= 1600 - 45){   // 水平方向防止出界
        if (player_x + Width / 2 - map_x <= 0){
            player_x ++;
        }
        else if (player_x + Width / 2 - map_x >= 1600 - 45){
            player_x --;
        }
    }

    if (player_y + High / 2 - map_y <= 20 || player_y + High / 2 - map_y >= 480){   // 竖直方向防止出界
        if (player_y + High / 2 - map_y <= 20){
            player_y ++;
        }
        else if (player_y + High / 2 - map_y >= 480){
            player_y --;
        }
    }

}

void touch_zombie_wall(){
    int i;
    for (i = 0 ; i < zombie_num; i ++){
        if (zombie_alive[i] == 1){
            if (zombie_x[i] + 72 > wall_x_a && zombie_x[i] < wall_x_b){       //在墙的竖直范围内 判断上下方向的碰撞
                if (zombie_y[i] + 100 > wall_y_a && zombie_y[i] < wall_y_a + 20)
                    zombie_y[i] --;
                if (zombie_y[i] < wall_y_b && zombie_y[i] > wall_y_b - 20)
                    zombie_y[i] ++;
            }
        }
    }
}


void touch(){
    touch_zombie_zombie();
    touch_player_wall();
    touch_zombie_wall();
}
/*---------------------------------------扑街模块-----------------------------------------*/
void gameover_restart()  // 重新开始模块(弃用)
{
    void play();
    little_loading();
    loading();
    startup();    // 重新加载
    play();
}

void gameover_back_to_menu()   // 回到主菜单
{
    void play_pvp();
    void play();
    mciSendString("close bk", NULL, 0, NULL);
    startup();
    menu();
    if (pvp == 0){
        play();
    }
    else if (pvp == 1){
        play_pvp();
    }
}

int gameover_score_pic()  //分数转化为图片
{
    int p;
    int k;
    int i;
    i = 0;
    p = score;
    while (p != 0){
        k = p % 10;
        if (k == 0)
        {
            img_num[i] = img_0;
            img_num_cover[i] = img_0_cover;
        }
        else if (k == 1)
        {
            img_num[i] = img_1;
            img_num_cover[i] = img_1_cover;
        }
        else if (k == 2)
        {
            img_num[i] = img_2;
            img_num_cover[i] = img_2_cover;
        }
        else if (k == 3)
        {
            img_num[i] = img_3;
            img_num_cover[i] = img_3_cover;
        }
        else if (k == 4)
        {
            img_num[i] = img_4;
            img_num_cover[i] = img_4_cover;
        }
        else if (k == 5)
        {
            img_num[i] = img_5;
            img_num_cover[i] = img_5_cover;
        }
        else if (k == 6)
        {
            img_num[i] = img_6;
            img_num_cover[i] = img_6_cover;
        }
        else if (k == 7)
        {
            img_num[i] = img_7;
            img_num_cover[i] = img_7_cover;
        }
        else if (k == 8)
        {
            img_num[i] = img_8;
            img_num_cover[i] = img_8_cover;
        }
        else if (k == 9)
        {
            img_num[i] = img_9;
            img_num_cover[i] = img_9_cover;
        }
        p = p / 10;
        i ++;
    }
    return i + 1;   // 传回分数位数

}

void gameover_quit()     // 退出游戏
{
    EndBatchDraw();
    mciSendString("close bk", NULL, 0, NULL);
    closegraph();
    exit(0);// 游戏结束
}

void gameover_show(int n)
{
    int i;
    int a;
    a = rand() % 5;
    int b;
    b = rand() % 5;
    putimage (0, 0, &img_gameover);
    if (pvp == 1){
        settextstyle(40, 0, _T("宋体"));
        outtextxy(Width / 2 - 90, 80, "YOU LOSE!");
        settextstyle(15, 0, _T("宋体"));
    }

    if (pick_quit == 1){       // quit菜单
        if (sound_switch == 1){
            mciSendString("open bm_music\\pass_button.mp3 alias pass", NULL, 0, NULL);//鼠标经过按键的声音
            mciSendString("play pass", NULL, 0, NULL);
        }
        putimage (227 + 110, 450, &img_quit_cover,NOTSRCERASE);               //特效
        putimage (227 + 110 + a, 450 + b, &img_quit_pick,SRCINVERT);
    }
    else{
        putimage (227 + 110, 450, &img_quit_cover,NOTSRCERASE);
        putimage (227 + 110, 450, &img_quit,SRCINVERT);
    }

    if (pick_backtomenu == 1){
        if (sound_switch == 1){
            mciSendString("open bm_music\\pass_button.mp3 alias pass", NULL, 0, NULL);//鼠标经过按键的声音
            mciSendString("play pass", NULL, 0, NULL);
        }
        putimage (207, 380, &img_backtomenu_cover,NOTSRCERASE);               //特效
        putimage (207 + a, 380 + b, &img_backtomenu_pick,SRCINVERT);
    }
    else{
        putimage (207, 380, &img_backtomenu_cover,NOTSRCERASE);
        putimage (207, 380, &img_backtomenu,SRCINVERT);
    }

    if (n == 1)
    {
        putimage (450, High / 2 - 28, &img_0_cover, NOTSRCERASE);
        putimage (450, High / 2 - 28, &img_0, SRCINVERT);
    }

    for (i = 0; i < n; i ++)   //显示分数
    {
        putimage (750 - i * 48 - (9 - n) * 45, High / 2 - 28, &img_num_cover[i], NOTSRCERASE);
        putimage (750 - i * 48 - (9 - n) * 45, High / 2 - 28, &img_num[i], SRCINVERT);
    }
    FlushBatchDraw();
}

void gameover_rank()    // 打破纪录 留下名字
{
    FILE *fp;
    int rank_old;   // 之前的纪录
    fp = fopen(".\\rank.zsc","r");
    fscanf(fp,"最高分：%d", &rank_old);
    fclose(fp);
    if (score > rank_old){
        settextcolor(BLUE);  //更改字体样式
        settextstyle(45, 0, _T("宋体"));
        outtextxy(Width / 2 - 170, High / 2 - 40, "恭喜你，打破纪录！");
        settextcolor(YELLOW);  //更改字体样式
        settextstyle(15, 0, _T("宋体"));
        FlushBatchDraw();
        fp = fopen(".\\rank.zsc","w");
        fprintf (fp,"最高分：%d", score);
        fclose(fp);
        Sleep(5000);
    }
}

void gameover_user() //用户选择
{
    while (MouseHit())
    {
        m=GetMouseMsg();
        if (m.x >= 227 + 110 && m.x <= 227 + 110 + 120 && m.y >= 450 && m.y <= 450 + 39)    // 点击quit
        {
            pick_quit= 1;
            if (m.uMsg == WM_LBUTTONDOWN)
            {
                gameover_quit();   //  退出游戏
            }

        }
        else{
            pick_quit = 0;
        }

        if (m.x >= 207 && m.x <= 207 + 410 && m.y >= 380 && m.y <= 380 + 39)   // 点击backtomenu
        {
            pick_backtomenu = 1;
            if (m.uMsg == WM_LBUTTONDOWN)
            {
                little_loading();
                pvp = 0;
                gameover_back_to_menu();
            }
        }
        else{
            pick_backtomenu = 0;
        }

    }
}

void gameover(){
    int n;
    if (player_hp <= 0){           // 生命值为0
        mciSendString("open bm_music\\ko.mp3 alias ko", NULL, 0, NULL);//ko音效
        mciSendString("play ko", NULL, 0, NULL);
        Sleep(2000);
        mciSendString("close ko", NULL, 0, NULL);
        gameover_rank();
        n = gameover_score_pic();
        while (1){
            gameover_show(n);
            gameover_user();
        }
    }
}

/*---------------------------------------------------------------------------------------*/
void update_wall() // 更新墙的位置
{
    wall_x_a = map_x - 100;
    wall_x_b = map_x + 2400;
    wall_y_a = map_y + 530;
    wall_y_b = map_y + 600;
}

void combo_count(int i)   // 连击计算
{
    if (i == 1){     // 传入1 连击增加
        combo ++;
        combo_time = combo_time_limit;
    }
    else{
        combo_time --;
        if (combo_time < 0){
            combo_time = 0;
            if (combo > 1){
                combo --;
                combo_time = combo_time_limit;
            }
        }
    }

}

void zombie_die(int i)   // 僵尸死亡判定
{
    void redbox_drop(int x, int y);
    int p;
    if (zombie_hp[i] <= 0){       // 僵尸死亡
        score = score + combo * 100;          // 分数统计
        combo_count(1);       // 传入1 ，连击增加
        dazhao_xuli ++;                    // 积累大招怒气
        p = rand() % 10;       // 十分之一概率掉落红盒子
        if (p == 0){
            redbox_drop(zombie_x[i], zombie_y[i]);
        }
        zombie_x[i] = -5000;
        zombie_y[i] = -5000;
        zombie_alive[i] = 0;  // 标记死亡僵尸
    }
}

void zombie_attacked() // 僵尸受击
{
    int i;
    int distance_s;
    int distance_w;
    int distance_d;
    int distance_a;      // 用于计算距离最近的僵尸距离
    char direction;      // 记录打击方向
    direction = 'o';     // o无意义
    distance_a = 5000;
    distance_d = 5000;
    distance_s = 5000;
    distance_w = 5000;   // 5000无意义
    int nearest;       // 用于标记最近的僵尸
    nearest = 5000;     // 5000无意义
    for (i = 0 ; i < zombie_num; i ++){
        if (zombie_alive[i] == 1 && hit == 0){
            if (fire == 1){
                if (player_direction == 's'){       // 向下射击
                    if ( abs (player_x + Width / 2 - map_x - zombie_x[i]) < 20 &&
                         -(player_y + High / 2 - map_y - zombie_y[i]) < gun_present.fire_length &&
                         -(player_y + High / 2 - map_y - zombie_y[i]) > 0){       // 判断是否打中
                        if (abs (player_y + High / 2 - map_y - zombie_y[i]) < distance_s){
                            distance_s = abs (player_y + High / 2 - map_y - zombie_y[i]);
                            nearest = i;
                        }
                        direction = 's';
                    }
                }
                else if (player_direction == 'a'){       // 向左射击
                    if ( abs (player_y + High / 2 - map_y - zombie_y[i]) < 30 &&
                         (player_x + Width / 2 - map_x - zombie_x[i]) < gun_present.fire_length &&
                         (player_x + Width / 2 - map_x - zombie_x[i]) > 0){
                        if (abs(player_x + Width / 2 - map_x - zombie_x[i]) < distance_a){
                            distance_a = abs(player_x + Width / 2 - map_x - zombie_x[i]);
                            nearest = i;
                        }
                        direction = 'a';
                    }

                }
                else if (player_direction == 'w'){       // 向上射击
                    if ( abs (player_x + Width / 2 - map_x - zombie_x[i]) < 20 &&
                         (player_y + High / 2 - map_y - zombie_y[i]) < gun_present.fire_length &&
                         (player_y + High / 2 - map_y - zombie_y[i]) > 0){
                        if (abs (player_y + High / 2 - map_y - zombie_y[i]) < distance_w){
                            distance_w = abs (player_y + High / 2 - map_y - zombie_y[i]);
                            nearest = i;
                        }
                        direction = 'w';
                    }

                }
                else if (player_direction == 'd'){       // 向右射击
                    if ( abs (player_y + High / 2 - map_y - zombie_y[i]) < 30 &&
                         -(player_x + Width / 2 - map_x - zombie_x[i]) < gun_present.fire_length &&
                         -(player_x + Width / 2 - map_x - zombie_x[i]) > 0){
                        if (abs (player_x + Width / 2 - map_x - zombie_x[i]) < distance_d){
                            distance_d = abs(player_x + Width / 2 - map_x - zombie_x[i]);
                            nearest = i;
                        }
                        direction = 'd';
                    }

                }
            }
        }
    }
    if (nearest != 5000 && direction != 'o'){
        zombie_hp[nearest] = zombie_hp[nearest] - gun_present.power;

        zombie_die(nearest);
        if (direction == 'd'){
            zombie_x[nearest] = zombie_x[nearest] + 20;
        }
        else if (direction == 'a'){
            zombie_x[nearest] = zombie_x[nearest] - 20;
        }
        else if (direction == 'w'){
            zombie_y[nearest] = zombie_y[nearest] - 20;
        }
        else if (direction == 's'){
            zombie_y[nearest] = zombie_y[nearest] + 20;
        }
        hit = 1;
    }
}

void update_gun_bullet()   // 计算换弹夹
{

}
void update_gun()
{
    if (player_direction == 'a'){           //调整枪的朝向
        img_gun = img_gun_a;
        img_gun_cover = img_gun_a_cover;
    }
    else if (player_direction == 'd'){
        img_gun = img_gun_d;
        img_gun_cover = img_gun_d_cover;
    }
    else if (player_direction == 's'){
        img_gun = img_gun_s;
        img_gun_cover = img_gun_s_cover;
    }
    else if (player_direction == 'w'){
        img_gun = img_gun_w;
        img_gun_cover = img_gun_w_cover;
    }

    if (gun_present.fire_time_count == gun_present.fire_time){                  // 计算子弹停留时间
        gun_present.fire_time_count = 0;
        fire = 0;

    }
    if (fire == 1){
        gun_present.fire_time_count ++;
        gun_present.gun_stop = 0;
        if (gun_present.gun_music_count >= gun_present.gun_music){
            mciSendString("close gun", NULL, 0, NULL);
            gun_present.gun_music_count = 0;
            mciSendString("close gun_prepare", NULL, 0, NULL);
        }
    }
    else
        gun_present.gun_stop ++;
    gun_present.gun_music_count ++;                  // 计算关闭枪声
    update_gun_bullet();

}

void update_dazhao()     // 更新大招状态 射速增加
{
    if (dazhao == 1)
    {
        dazhao_xuli = 0;
        dazhao_time--;
        if (dazhao_time <= 0)
        {
            dazhao = 0;
            gun_present.gun_speed = gun_present.gun_speed * 3; // 恢复射速
            mciSendString("close dazhao", NULL, 0, NULL);
        }

    }
}

void update_levelup()   // 升级
{
    if (combo / 5 + 1> level){
        level = combo / 5 + 1;    // 升级出现更多僵尸
        if (zombie_num + 2 <= zombie_num_limit){
            zombie_num = zombie_num + 2;
        }
    }

}

void redbox_drop(int x, int y)  // 僵尸掉落红盒子
{
    int i;
    for (i = 0; i < redbox_num_limit; i ++){
        if (rb[i].exist == 0){
            rb[i].x = x;
            rb[i].y = y;
            rb[i].exist = 1;
            rb[i].pick = 0;
            break;
        }
    }

}
void update_redbox()// 检测盒子是否被拾取以及拾取效果
{
    int i;
    int k;
    for (i = 0; i < redbox_num; i ++){
        if (player_y + High / 2 - map_y + 67 > rb[i].y && player_y + High / 2 - map_y + 67 < rb[i].y + 29
            && player_x + Width / 2 - map_x + 45 > rb[i].x + 10 && player_x + Width / 2 - map_x < rb[i].x + 30 - 10){
            if (sound_switch == 1){
                mciSendString("close pick", NULL, 0, NULL);
                mciSendString("open bm_music\\pick_redbox.mp3 alias pick", NULL, 0, NULL);//进入游戏音效
                mciSendString("play pick", NULL, 0, NULL);
            }
            rb[i].pick = 1;    // 盒子失效
            rb[i].exist = 0;
            rb[i].x = -5000;
            rb[i].y = -5000;
            k = rand() % 2;
            if (k == 0){        // 随机效果
                player_hp = 100;
            }
            else if (k == 1){
                gun_present.bullet = gun_present.bullet_max;
            }
        }
    }
}

void update ()
{
    hold_map();
    update_wall();
    update_walk();
    zombie_attack();
    zombie_move();
    zombie_birth();
    touch();
    change_gun();
    zombie_attacked();
    update_gun();
    update_dazhao();
    combo_count(0);
    update_levelup();
    update_redbox();

    gameover();
}

/*========================================显示部分===========================================*/
/*========================================显示部分===========================================*/
void show_zombie()
{
    int i;

    for (i = 0; i < zombie_num; i ++){
        if (zombie_alive[i] == 1){
            putimage(map_x + zombie_x[i], map_y + zombie_y[i], 45, 67, &img_zombie_cover[i], zombie_walk[i] * 45, 0,NOTSRCERASE);
            putimage(map_x + zombie_x[i], map_y + zombie_y[i], 45, 67, &img_zombie[i], zombie_walk[i] * 45, 0,SRCINVERT);
        }
    }
}

void show_player_hp()
{
    putimage(Width / 2 + player_x - 10, High / 2 + player_y - 15, &img_hp);
    putimage(Width / 2 + player_x - 10 + 63 - (int)((100 - player_hp) * 0.62) , High / 2 + player_y - 15 + 1,
             (int)((100 - player_hp) * 0.62), 8,&img_hp_cover, 0, 0);
}

void show_gun()
{
    int plus;
    if (player_walk == 0){                  // 抢的位置随着人物走动变化
        plus = 0;
    }
    else if (player_walk == 1){
        plus = 3;
    }
    else if (player_walk == 2){
        plus = 0;
    }
    else if (player_walk == 3){
        plus = -3;
    }

    if (gun_model == 1){      // 手枪
        if (player_direction == 'a'){
            putimage(Width / 2 + player_x + 5 + plus, High / 2 + player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(Width / 2 + player_x + 5 + plus, High / 2 + player_y + 33, &img_gun, SRCINVERT);
        }
        else if (player_direction == 'd'){
            putimage(Width / 2 + player_x + 20 + plus, High / 2 + player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(Width / 2 + player_x + 20 + plus, High / 2 + player_y + 33, &img_gun, SRCINVERT);
        }
        else if (player_direction == 's'){
            putimage(Width / 2 + player_x + 6 + player_walk % 2, High / 2 + player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(Width / 2 + player_x + 6 + player_walk % 2, High / 2 + player_y + 33, &img_gun, SRCINVERT);
        }
        else if (player_direction == 'w'){
            putimage(Width / 2 + player_x + 35 + player_walk % 2, High / 2 + player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(Width / 2 + player_x + 35 + player_walk % 2, High / 2 + player_y + 33, &img_gun, SRCINVERT);
        }
    }
    else if (gun_model == 2){      //uzi
        if (player_direction == 'a'){
            putimage(Width / 2 + player_x + 5 + plus - 2, High / 2 + player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(Width / 2 + player_x + 5 + plus - 2, High / 2 + player_y + 33, &img_gun, SRCINVERT);
        }
        else if (player_direction == 'd'){
            putimage(Width / 2 + player_x + 20 + plus - 5, High / 2 + player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(Width / 2 + player_x + 20 + plus - 5, High / 2 + player_y + 33, &img_gun, SRCINVERT);
        }
        else if (player_direction == 's'){
            putimage(Width / 2 + player_x + 6 + player_walk % 2, High / 2 + player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(Width / 2 + player_x + 6 + player_walk % 2, High / 2 + player_y + 33, &img_gun, SRCINVERT);
        }
        else if (player_direction == 'w'){
            putimage(Width / 2 + player_x + 35 + player_walk % 2, High / 2 + player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(Width / 2 + player_x + 35 + player_walk % 2, High / 2 + player_y + 33, &img_gun, SRCINVERT);
        }
    }
    else if (gun_model == 3){      //shotgun
        if (player_direction == 'a'){
            putimage(Width / 2 + player_x + 5 + plus - 13, High / 2 + player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(Width / 2 + player_x + 5 + plus - 13, High / 2 + player_y + 33, &img_gun, SRCINVERT);
        }
        else if (player_direction == 'd'){
            putimage(Width / 2 + player_x + 20 + plus - 5, High / 2 + player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(Width / 2 + player_x + 20 + plus - 5, High / 2 + player_y + 33, &img_gun, SRCINVERT);
        }
        else if (player_direction == 's'){
            putimage(Width / 2 + player_x + 6 + player_walk % 2, High / 2 + player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(Width / 2 + player_x + 6 + player_walk % 2, High / 2 + player_y + 33, &img_gun, SRCINVERT);
        }
        else if (player_direction == 'w'){
            putimage(Width / 2 + player_x + 35 + player_walk % 2, High / 2 + player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(Width / 2 + player_x + 35 + player_walk % 2, High / 2 + player_y + 33, &img_gun, SRCINVERT);
        }
    }

}
void show_player()
{
    putimage(Width / 2 + player_x, High / 2 + player_y, 45, 67, &img_player_cover, player_walk * 45, 0,NOTSRCERASE);
    putimage(Width / 2 + player_x, High / 2 + player_y, 45, 67, &img_player, player_walk * 45, 0,SRCINVERT);
    show_gun();
}

void show_wall()
{
    putimage(map_x, map_y + 465, &img_wall_cover, NOTSRCERASE);
    putimage(map_x, map_y + 465, &img_wall, SRCINVERT);
}

void show_dazhao()
{
    if (dazhao == 1)
    {
        putimage(Width / 2 + player_x - 50, High / 2 + player_y - 50, &img_dazhao);
    }
}

/*--------------------------------------显示枪焰-----------------------------------------*/
void show_fire_shotgun()
{
    setlinecolor (YELLOW);
    if (player_direction == 's'){       // 向下射击
        line (Width / 2 + player_x + 8, High / 2 + player_y + 35,
              Width / 2 + player_x + 8, High / 2 + player_y + 35 + gun_present.fire_length);
        line (Width / 2 + player_x + 8, High / 2 + player_y + 35,
              Width / 2 + player_x + 8 + 4, High / 2 + player_y + 35 + gun_present.fire_length);
        line (Width / 2 + player_x + 8, High / 2 + player_y + 35,
              Width / 2 + player_x + 8 - 4, High / 2 + player_y + 35 + gun_present.fire_length);
    }
    if (player_direction == 'a'){       // 向左射击
        line (Width / 2 + player_x - 7, High / 2 + player_y + 35,
              Width / 2 + player_x - 7 - gun_present.fire_length, High / 2 + player_y + 35);
        line (Width / 2 + player_x - 7, High / 2 + player_y + 35,
              Width / 2 + player_x - 7 - gun_present.fire_length, High / 2 + player_y + 35 + 4);
        line (Width / 2 + player_x - 7, High / 2 + player_y + 35,
              Width / 2 + player_x - 7 - gun_present.fire_length, High / 2 + player_y + 35 - 4);
    }
    if (player_direction == 'w'){       // 向上射击
        line (Width / 2 + player_x + 37, High / 2 + player_y + 35,
              Width / 2 + player_x + 37, High / 2 + player_y + 35 - gun_present.fire_length);
        line (Width / 2 + player_x + 37, High / 2 + player_y + 35,
              Width / 2 + player_x + 37 + 4, High / 2 + player_y + 35 - gun_present.fire_length);
        line (Width / 2 + player_x + 37, High / 2 + player_y + 35,
              Width / 2 + player_x + 37 - 4, High / 2 + player_y + 35 - gun_present.fire_length);
    }
    if (player_direction == 'd'){       // 向右射击
        line (Width / 2 + player_x + 55, High / 2 + player_y + 35,
              Width / 2 + player_x + 55 + gun_present.fire_length, High / 2 + player_y + 35);
        line (Width / 2 + player_x + 55, High / 2 + player_y + 35,
              Width / 2 + player_x + 55 + gun_present.fire_length, High / 2 + player_y + 35 + 4);
        line (Width / 2 + player_x + 55, High / 2 + player_y + 35,
              Width / 2 + player_x + 55 + gun_present.fire_length, High / 2 + player_y + 35 - 4);
    }
}

void show_fire_uzi()
{
    setlinecolor (YELLOW);
    if (player_direction == 's'){       // 向下射击
        line (Width / 2 + player_x + 8, High / 2 + player_y + 35,
              Width / 2 + player_x + 8, High / 2 + player_y + 35 + gun_present.fire_length);
    }
    if (player_direction == 'a'){       // 向左射击
        line (Width / 2 + player_x + 5, High / 2 + player_y + 35,
              Width / 2 + player_x + 5 - gun_present.fire_length, High / 2 + player_y + 35);
    }
    if (player_direction == 'w'){       // 向上射击
        line (Width / 2 + player_x + 37, High / 2 + player_y + 35,
              Width / 2 + player_x + 37, High / 2 + player_y + 35 - gun_present.fire_length);
    }
    if (player_direction == 'd'){       // 向右射击
        line (Width / 2 + player_x + 37, High / 2 + player_y + 35,
              Width / 2 + player_x + 37 + gun_present.fire_length, High / 2 + player_y + 35);
    }
}

void show_fire_handgun()
{
    setlinecolor (YELLOW);
    if (player_direction == 's'){       // 向下射击
        line (Width / 2 + player_x + 8, High / 2 + player_y + 35,
              Width / 2 + player_x + 8, High / 2 + player_y + 35 + gun_present.fire_length);
    }
    if (player_direction == 'a'){       // 向左射击
        line (Width / 2 + player_x + 5, High / 2 + player_y + 35,
              Width / 2 + player_x + 5 - gun_present.fire_length, High / 2 + player_y + 35);
    }
    if (player_direction == 'w'){       // 向上射击
        line (Width / 2 + player_x + 37, High / 2 + player_y + 35,
              Width / 2 + player_x + 37, High / 2 + player_y + 35 - gun_present.fire_length);
    }
    if (player_direction == 'd'){       // 向右射击
        line (Width / 2 + player_x + 37, High / 2 + player_y + 35,
              Width / 2 + player_x + 37 + gun_present.fire_length, High / 2 + player_y + 35);
    }
}

void show_fire()
{
    if (fire == 1){
        if (sound_switch == 1){
            if (gun_model == 3){
                mciSendString("open bm_music\\shotgun.mp3 alias gun", NULL, 0, NULL);//枪声
            }
            else if (gun_model == 2){
                mciSendString("open bm_music\\uzi.mp3 alias gun", NULL, 0, NULL);//枪声
            }
            else if (gun_model == 1){
                mciSendString("open bm_music\\handgun.mp3 alias gun", NULL, 0, NULL);//枪声
            }
            mciSendString("play gun", NULL, 0, NULL);
            mciSendString("open bm_music\\gun_prepare.mp3 alias gun_prepare", NULL, 0, NULL);    //上膛
            mciSendString("play gun_prepare", NULL, 0, NULL);
        }

        if (gun_model == 3){
            show_fire_shotgun();
        }
        else if (gun_model == 2){
            show_fire_uzi();
        }
        else if (gun_model == 1){
            show_fire_handgun();
        }


    }
    show_dazhao();
}
/*---------------------------------------------------------------------------------------------------*/

void show_gun_info()   // 显示枪的型号
{
    char b[5];
    char bm[5];
    sprintf (b, "%d", gun_present.bullet);
    sprintf (bm, "%d", gun_present.bullet_max);
    outtextxy(Width - 110, 20, "GUN:");
    if (gun_model == 1){
        outtextxy(Width - 70, 20, "handgun");
    }
    else if (gun_model == 2){
        outtextxy(Width - 70, 20, "uzi");
    }
    else if (gun_model == 3){
        outtextxy(Width - 70, 20, "shotgun");
    }
    outtextxy(Width - 110, 40, "BULLET:");
    outtextxy(Width - 55, 40, b);
    outtextxy(Width - 30, 40, "/");
    outtextxy(Width - 24, 40, bm);
}

int show_score_pic()  //分数转化为图片
{
    int p;
    int k;
    int i;
    i = 0;
    p = score;
    while (p != 0){
        k = p % 10;
        if (k == 0)
        {
            img_snum[i] = img_s0;
            img_snum_cover[i] = img_s0_cover;
        }
        else if (k == 1)
        {
            img_snum[i] = img_s1;
            img_snum_cover[i] = img_s1_cover;
        }
        else if (k == 2)
        {
            img_snum[i] = img_s2;
            img_snum_cover[i] = img_s2_cover;
        }
        else if (k == 3)
        {
            img_snum[i] = img_s3;
            img_snum_cover[i] = img_s3_cover;
        }
        else if (k == 4)
        {
            img_snum[i] = img_s4;
            img_snum_cover[i] = img_s4_cover;
        }
        else if (k == 5)
        {
            img_snum[i] = img_s5;
            img_snum_cover[i] = img_s5_cover;
        }
        else if (k == 6)
        {
            img_snum[i] = img_s6;
            img_snum_cover[i] = img_s6_cover;
        }
        else if (k == 7)
        {
            img_snum[i] = img_s7;
            img_snum_cover[i] = img_s7_cover;
        }
        else if (k == 8)
        {
            img_snum[i] = img_s8;
            img_snum_cover[i] = img_s8_cover;
        }
        else if (k == 9)
        {
            img_snum[i] = img_s9;
            img_snum_cover[i] = img_s9_cover;
        }
        p = p / 10;
        i ++;
    }
    return i + 1;   // 传回分数位数
}

void show_info()   // 显示基本信息：分数 combo
{
    int n;
    int i;
    char c[4];
    int p_x;
    int p_y;
    int len;
    double ct1;
    len = 24;
    n = show_score_pic();

    ct1 = (1.0 * combo_time / combo_time_limit) * 1.25 * PI - 0.25 * PI;     // 控制combo计数点的角度
    p_x = 531 + len * cos(ct1);
    p_y = 61 + len * sin(ct1);

    for (i = 0; i < 10; i ++){       //显示分数
        putimage (515 - i * 22, 20, &img_snum_cover[i], NOTSRCERASE);
        putimage (515 - i * 22, 20, &img_snum[i], SRCINVERT);
    }
    putimage (508, 37, &img_combo_cover, NOTSRCERASE);
    putimage (508, 37, &img_combo, SRCINVERT);

    putimage (p_x, p_y, &img_combo_p_cover, NOTSRCERASE);    // 显示连击时间
    putimage (p_x, p_y, &img_combo_p, SRCINVERT);

    sprintf(c, "%d", combo);
    settextcolor(WHITE);  //更改字体样式
    outtextxy(520, 56, "x");
    if (combo > 0 && combo < 10){
        outtextxy(535 , 57, c);
    }
    else if (combo > 9 && combo < 100){
        outtextxy(532 , 57, c);
    }
    else if (combo > 99){
        outtextxy(529 , 57, c);
    }
    settextcolor(YELLOW);

}

void show_redbox()//显示红盒子
{
    int i;
    for (i = 0; i < redbox_num; i ++){
        putimage (rb[i].x + map_x, rb[i].y + map_y, &img_redbox_cover, NOTSRCERASE);
        putimage (rb[i].x + map_x, rb[i].y + map_y, &img_redbox, SRCINVERT);
    }
}

void show_subtitle()  // 字幕信息
{

}

void show()
{
    putimage(map_x, map_y, &img_map);
    show_redbox();
    show_player();
    show_zombie();
    show_wall();
    show_player_hp();
    show_fire();
    show_info();
    show_gun_info();
    show_subtitle();

    FlushBatchDraw();

}

void play()
{
    while (1)
    {
        user();
        update();
        show();
    }
}
/*==========================================pvp=================================================*/
/*==========================================pvp=================================================*/
void pvp_show_fire_shotgun()
{
    setlinecolor (YELLOW);
    if (player_direction == 's'){       // 向下射击
        line (player_x + 8,player_y + 35,
              player_x + 8,player_y + 35 + gun_present.fire_length);
        line (player_x + 8,player_y + 35,
              player_x + 8 + 4, player_y + 35 + gun_present.fire_length);
        line (player_x + 8,player_y + 35,
              player_x + 8 - 4, player_y + 35 + gun_present.fire_length);
    }
    if (player_direction == 'a'){       // 向左射击
        line (player_x - 7, player_y + 35,
              player_x - 7 - gun_present.fire_length,player_y + 35);
        line (player_x - 7,player_y + 35,
              player_x - 7 - gun_present.fire_length,player_y + 35 + 4);
        line (player_x - 7,player_y + 35,
              player_x - 7 - gun_present.fire_length,player_y + 35 - 4);
    }
    if (player_direction == 'w'){       // 向上射击
        line (player_x + 37, player_y + 35,
              player_x + 37,player_y + 35 - gun_present.fire_length);
        line (player_x + 37, player_y + 35,
              player_x + 37 + 4,player_y + 35 - gun_present.fire_length);
        line (player_x + 37, player_y + 35,
              player_x + 37 - 4,player_y + 35 - gun_present.fire_length);
    }
    if (player_direction == 'd'){       // 向右射击
        line (player_x + 55, player_y + 35,
              player_x + 55 + gun_present.fire_length,player_y + 35);
        line (player_x + 55,player_y + 35,
              player_x + 55 + gun_present.fire_length,player_y + 35 + 4);
        line (player_x + 55,player_y + 35,
              player_x + 55 + gun_present.fire_length,player_y + 35 - 4);
    }
}

void pvp_show_fire_uzi()
{
    setlinecolor (YELLOW);
    if (player_direction == 's'){       // 向下射击
        line (player_x + 8, player_y + 35,
              player_x + 8,player_y + 35 + gun_present.fire_length);
    }
    if (player_direction == 'a'){       // 向左射击
        line (player_x + 5, player_y + 35,
              player_x + 5 - gun_present.fire_length, player_y + 35);
    }
    if (player_direction == 'w'){       // 向上射击
        line (player_x + 37,player_y + 35,
              player_x + 37, player_y + 35 - gun_present.fire_length);
    }
    if (player_direction == 'd'){       // 向右射击
        line (player_x + 37,player_y + 35,
              player_x + 37 + gun_present.fire_length, player_y + 35);
    }
}

void pvp_show_fire_handgun()
{
    setlinecolor (YELLOW);
    if (player_direction == 's'){       // 向下射击
        line (player_x + 8,player_y + 35,
              player_x + 8,player_y + 35 + gun_present.fire_length);
    }
    if (player_direction == 'a'){       // 向左射击
        line (player_x + 5, player_y + 35,
              player_x + 5 - gun_present.fire_length, player_y + 35);
    }
    if (player_direction == 'w'){       // 向上射击
        line (player_x + 37, player_y + 35,
              player_x + 37,player_y + 35 - gun_present.fire_length);
    }
    if (player_direction == 'd'){       // 向右射击
        line (player_x + 37,player_y + 35,
              player_x + 37 + gun_present.fire_length,player_y + 35);
    }
}

void pvp_show_fire()
{
    if (fire == 1){
        if (sound_switch == 1){
            if (gun_model == 3){
                mciSendString("open bm_music\\shotgun.mp3 alias gun", NULL, 0, NULL);//枪声
            }
            else if (gun_model == 2){
                mciSendString("open bm_music\\uzi.mp3 alias gun", NULL, 0, NULL);//枪声
            }
            else if (gun_model == 1){
                mciSendString("open bm_music\\handgun.mp3 alias gun", NULL, 0, NULL);//枪声
            }
            mciSendString("play gun", NULL, 0, NULL);
            mciSendString("open bm_music\\gun_prepare.mp3 alias gun_prepare", NULL, 0, NULL);    //上膛
            mciSendString("play gun_prepare", NULL, 0, NULL);
        }

        if (gun_model == 3){
            pvp_show_fire_shotgun();
        }
        else if (gun_model == 2){
            pvp_show_fire_uzi();
        }
        else if (gun_model == 1){
            pvp_show_fire_handgun();
        }
    }

}

void pvp_show_player_hp()
{
    putimage(player_x - 10,player_y - 15, &img_hp);
    putimage(player_x - 10 + 63 - (int)((100 - player_hp) * 0.62) ,player_y - 15 + 1,
             (int)((100 - player_hp) * 0.62), 8,&img_hp_cover, 0, 0);
}

void pvp_show_enemy_hp()
{
    putimage(enemy_x - 10,enemy_y - 15, &img_enemy_hp);
    putimage(enemy_x - 10 + 63 - (int)((100 - enemy_hp) * 0.62) ,enemy_y - 15 + 1,
             (int)((100 - enemy_hp) * 0.62), 8,&img_hp_cover, 0, 0);
}

void pvp_hit()  //检测有没有打中敌人
{
    if (fire == 1 && hit == 0){
        if (player_direction == 's'){       // 向下射击
            if ( abs (player_x - enemy_x) < 20 &&
                 -(player_y - enemy_y) < gun_present.fire_length &&
                 -(player_y - enemy_y) > 0){       // 判断是否打中
                hit_enemy = gun_present.power;     // 打中敌人的血量
                score = score + gun_present.power * 100;
                hit = 1;
            }
        }
        else if (player_direction == 'a'){       // 向左射击
            if ( abs (player_y - enemy_y) < 30 &&
                 (player_x - enemy_x) < gun_present.fire_length &&
                 (player_x - enemy_x) > 0){
                hit_enemy = gun_present.power;
                score = score + gun_present.power * 100;
                hit = 1;
            }

        }
        else if (player_direction == 'w'){       // 向上射击
            if ( abs (player_x - enemy_x) < 20 &&
                 (player_y - enemy_y) < gun_present.fire_length &&
                 (player_y - enemy_y) > 0){
                hit_enemy = gun_present.power;
                score = score + gun_present.power * 100;
                hit = 1;
            }

        }
        else if (player_direction == 'd'){       // 向右射击
            if ( abs (player_y - enemy_y) < 30 &&
                 -(player_x - enemy_x) < gun_present.fire_length &&
                 -(player_x - enemy_x) > 0){
                hit_enemy = gun_present.power;
                score = score + gun_present.power * 100;
                hit = 1;
            }
        }
    }
}

void pvp_touch()//撞墙
{
    if (player_x > 800 - 45){
        player_x = 800 - 45;
    }
    if (player_x < 0){
        player_x = 0;
    }
    if (player_y < 0){
        player_y = 0;
    }
    if (player_y > 600 - 100){
        player_y = 600 - 100;
    }

}
void pvp_show_player()
{
    void pvp_show_gun();
    putimage(player_x, player_y, 45, 67, &img_player_cover, player_walk * 45, 0,NOTSRCERASE);
    putimage(player_x, player_y, 45, 67, &img_player, player_walk * 45, 0,SRCINVERT);
    pvp_show_gun();

}

void pvp_show_enemy_gun()   // 显示敌人的枪
{
    if (enemy_gun_model == 1){      // 手枪
        if (enemy_direction == 'a'){
            putimage(enemy_x + 5, enemy_y + 33, &img_enemy_gun_cover, NOTSRCERASE);
            putimage(enemy_x + 5, enemy_y + 33, &img_enemy_gun, SRCINVERT);
        }
        else if (enemy_direction == 'd'){
            putimage(enemy_x + 20, enemy_y + 33, &img_enemy_gun_cover, NOTSRCERASE);
            putimage(enemy_x + 20, enemy_y + 33, &img_enemy_gun, SRCINVERT);
        }
        else if (enemy_direction == 's'){
            putimage(enemy_x + 6, enemy_y + 33, &img_enemy_gun_cover, NOTSRCERASE);
            putimage(enemy_x + 6, enemy_y + 33, &img_enemy_gun, SRCINVERT);
        }
        else if (enemy_direction == 'w'){
            putimage(enemy_x + 35, enemy_y + 33, &img_enemy_gun_cover, NOTSRCERASE);
            putimage(enemy_x + 35, enemy_y + 33, &img_enemy_gun, SRCINVERT);
        }
    }
    else if (enemy_gun_model == 2){      //uzi
        if (enemy_direction == 'a'){
            putimage(enemy_x + 5 - 2, enemy_y + 33, &img_enemy_gun_cover, NOTSRCERASE);
            putimage(enemy_x + 5 - 2, enemy_y + 33, &img_enemy_gun, SRCINVERT);
        }
        else if (enemy_direction == 'd'){
            putimage(enemy_x + 20 - 5, enemy_y + 33, &img_enemy_gun_cover, NOTSRCERASE);
            putimage(enemy_x + 20 - 5, enemy_y + 33, &img_enemy_gun, SRCINVERT);
        }
        else if (enemy_direction == 's'){
            putimage(enemy_x + 6, enemy_y + 33, &img_enemy_gun_cover, NOTSRCERASE);
            putimage(enemy_x + 6, enemy_y + 33, &img_enemy_gun, SRCINVERT);
        }
        else if (enemy_direction == 'w'){
            putimage(enemy_x + 35, enemy_y + 33, &img_enemy_gun_cover, NOTSRCERASE);
            putimage(enemy_x + 35, enemy_y + 33, &img_enemy_gun, SRCINVERT);
        }
    }
    else if (enemy_gun_model == 3){      //shotgun
        if (enemy_direction == 'a'){
            putimage(enemy_x + 5 - 13, enemy_y + 33, &img_enemy_gun_cover, NOTSRCERASE);
            putimage(enemy_x + 5 - 13, enemy_y + 33, &img_enemy_gun, SRCINVERT);
        }
        else if (enemy_direction == 'd'){
            putimage(enemy_x + 20 - 5, enemy_y + 33, &img_enemy_gun_cover, NOTSRCERASE);
            putimage(enemy_x + 20 - 5, enemy_y + 33, &img_enemy_gun, SRCINVERT);
        }
        else if (enemy_direction == 's'){
            putimage(enemy_x + 6, enemy_y + 33, &img_enemy_gun_cover, NOTSRCERASE);
            putimage(enemy_x + 6, enemy_y + 33, &img_enemy_gun, SRCINVERT);
        }
        else if (enemy_direction == 'w'){
            putimage(enemy_x + 35, enemy_y + 33, &img_enemy_gun_cover, NOTSRCERASE);
            putimage(enemy_x + 35, enemy_y + 33, &img_enemy_gun, SRCINVERT);
        }
    }
}

void pvp_show_enemy() //显示敌人
{
    putimage (map_x + enemy_x, map_y + enemy_y, 45, 67, &img_enemy_cover, enemy_walk * 45, 0, NOTSRCERASE);
    putimage (map_x + enemy_x, map_y + enemy_y, 45, 67, &img_enemy, enemy_walk * 45, 0, SRCINVERT);
    pvp_show_enemy_gun();
}

void pvp_show_gun()
{
    int plus;
    if (player_walk == 0){                  // 抢的位置随着人物走动变化
        plus = 0;
    }
    else if (player_walk == 1){
        plus = 3;
    }
    else if (player_walk == 2){
        plus = 0;
    }
    else if (player_walk == 3){
        plus = -3;
    }

    if (gun_model == 1){      // 手枪
        if (player_direction == 'a'){
            putimage(player_x + 5 + plus,player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(player_x + 5 + plus,player_y + 33, &img_gun, SRCINVERT);
        }
        else if (player_direction == 'd'){
            putimage(player_x + 20 + plus,player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(player_x + 20 + plus, player_y + 33, &img_gun, SRCINVERT);
        }
        else if (player_direction == 's'){
            putimage(player_x + 6 + player_walk % 2,player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(player_x + 6 + player_walk % 2,player_y + 33, &img_gun, SRCINVERT);
        }
        else if (player_direction == 'w'){
            putimage(player_x + 35 + player_walk % 2,player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(player_x + 35 + player_walk % 2,player_y + 33, &img_gun, SRCINVERT);
        }
    }
    else if (gun_model == 2){      //uzi
        if (player_direction == 'a'){
            putimage(player_x + 5 + plus - 2,player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(player_x + 5 + plus - 2,player_y + 33, &img_gun, SRCINVERT);
        }
        else if (player_direction == 'd'){
            putimage(player_x + 20 + plus - 5,player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(player_x + 20 + plus - 5,player_y + 33, &img_gun, SRCINVERT);
        }
        else if (player_direction == 's'){
            putimage(player_x + 6 + player_walk % 2,player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(player_x + 6 + player_walk % 2,player_y + 33, &img_gun, SRCINVERT);
        }
        else if (player_direction == 'w'){
            putimage(player_x + 35 + player_walk % 2,player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(player_x + 35 + player_walk % 2,player_y + 33, &img_gun, SRCINVERT);
        }
    }
    else if (gun_model == 3){      //shotgun
        if (player_direction == 'a'){
            putimage(player_x + 5 + plus - 13,player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(player_x + 5 + plus - 13,player_y + 33, &img_gun, SRCINVERT);
        }
        else if (player_direction == 'd'){
            putimage(player_x + 20 + plus - 5,player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(player_x + 20 + plus - 5,player_y + 33, &img_gun, SRCINVERT);
        }
        else if (player_direction == 's'){
            putimage(player_x + 6 + player_walk % 2,player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(player_x + 6 + player_walk % 2,player_y + 33, &img_gun, SRCINVERT);
        }
        else if (player_direction == 'w'){
            putimage(player_x + 35 + player_walk % 2,player_y + 33, &img_gun_cover, NOTSRCERASE);
            putimage(player_x + 35 + player_walk % 2,player_y + 33, &img_gun, SRCINVERT);
        }
    }
}

void pvp_show_enemy_fire_shotgun()
{
    setlinecolor (YELLOW);
    if (enemy_direction == 's'){       // 向下射击
        line (enemy_x + 8,enemy_y + 35,
              enemy_x + 8,enemy_y + 35 + enemy_fire_len);
        line (enemy_x + 8,enemy_y + 35,
              enemy_x + 8 + 4, enemy_y + 35 + enemy_fire_len);
        line (enemy_x + 8, enemy_y + 35,
              enemy_x + 8 - 4, enemy_y + 35 + enemy_fire_len);
    }
    if (enemy_direction == 'a'){       // 向左射击
        line (enemy_x - 7, enemy_y + 35,
              enemy_x - 7 - enemy_fire_len,enemy_y + 35);
        line (enemy_x - 7,enemy_y + 35,
              enemy_x - 7 - enemy_fire_len,enemy_y + 35 + 4);
        line (enemy_x - 7,enemy_y + 35,
              enemy_x - 7 - enemy_fire_len,enemy_y + 35 - 4);
    }
    if (enemy_direction == 'w'){       // 向上射击
        line (enemy_x + 37, enemy_y + 35,
              enemy_x + 37,enemy_y + 35 - enemy_fire_len);
        line (enemy_x + 37, enemy_y + 35,
              enemy_x + 37 + 4,enemy_y + 35 - enemy_fire_len);
        line (enemy_x + 37, enemy_y + 35,
              enemy_x + 37 - 4,enemy_y + 35 - enemy_fire_len);
    }
    if (enemy_direction == 'd'){       // 向右射击
        line (enemy_x + 55, enemy_y + 35,
              enemy_x + 55 + enemy_fire_len,enemy_y + 35);
        line (enemy_x + 55,enemy_y + 35,
              enemy_x + 55 + enemy_fire_len,enemy_y + 35 + 4);
        line (enemy_x + 55,enemy_y + 35,
              enemy_x + 55 + enemy_fire_len,enemy_y + 35 - 4);
    }
}

void pvp_show_enemy_fire_uzi()
{
    setlinecolor (YELLOW);
    if (enemy_direction == 's'){       // 向下射击
        line (enemy_x + 8, enemy_y + 35,
              enemy_x + 8,enemy_y + 35 + enemy_fire_len);
    }
    if (enemy_direction == 'a'){       // 向左射击
        line (enemy_x + 5, enemy_y + 35,
              enemy_x + 5 - enemy_fire_len, enemy_y + 35);
    }
    if (enemy_direction == 'w'){       // 向上射击
        line (enemy_x + 37,enemy_y + 35,
              enemy_x + 37, enemy_y + 35 - enemy_fire_len);
    }
    if (enemy_direction == 'd'){       // 向右射击
        line (enemy_x + 37,enemy_y + 35,
              enemy_x + 37 + enemy_fire_len, enemy_y + 35);
    }
}

void pvp_show_enemy_fire_handgun()
{
    setlinecolor (YELLOW);
    if (enemy_direction == 's'){       // 向下射击
        line (enemy_x + 8,enemy_y + 35,
              enemy_x + 8,enemy_y + 35 + enemy_fire_len);
    }
    if (enemy_direction == 'a'){       // 向左射击
        line (enemy_x + 5, enemy_y + 35,
              enemy_x + 5 - enemy_fire_len, enemy_y + 35);
    }
    if (enemy_direction == 'w'){       // 向上射击
        line (enemy_x + 37, enemy_y + 35,
              enemy_x + 37,enemy_y + 35 - enemy_fire_len);
    }
    if (enemy_direction == 'd'){       // 向右射击
        line (enemy_x + 37,enemy_y + 35,
              enemy_x + 37 + enemy_fire_len,enemy_y + 35);
    }
}

void pvp_show_enemy_fire()
{
    if (enemy_fire == 1){
        /*
        if (sound_switch == 1){
                    if (enemy_gun_model == 3){
                        mciSendString("open bm_music\\shotgun.mp3 alias gun", NULL, 0, NULL);//枪声
                    }
                    else if (enemy_gun_model == 2){
                        mciSendString("open bm_music\\uzi.mp3 alias gun", NULL, 0, NULL);//枪声
                    }
                    else if (enemy_gun_model == 1){
                        mciSendString("open bm_music\\handgun.mp3 alias gun", NULL, 0, NULL);//枪声
                    }
                    mciSendString("play gun", NULL, 0, NULL);
                    mciSendString("open bm_music\\gun_prepare.mp3 alias gun_prepare", NULL, 0, NULL);    //上膛
                    mciSendString("play gun_prepare", NULL, 0, NULL);
                }*/

        if (enemy_gun_model == 3){
            pvp_show_enemy_fire_shotgun();
        }
        else if (enemy_gun_model == 2){
            pvp_show_enemy_fire_uzi();
        }
        else if (enemy_gun_model == 1){
            pvp_show_enemy_fire_handgun();
        }
    }

}

void pvp_show()
{
    putimage(0, 0, &img_map);

    pvp_show_player();
    pvp_show_enemy();
    show_wall();
    pvp_show_player_hp();
    pvp_show_enemy_hp();
    pvp_show_fire();
    pvp_show_enemy_fire();
    show_gun_info();
    show_subtitle();

    FlushBatchDraw();
}
/*----------------------------------------------------------------------------------------*/
void pvp_win_show(int n)
{
    int i;
    int a;
    a = rand() % 5;
    int b;
    b = rand() % 5;
    putimage (0, 0, &img_gameover);
    settextstyle(40, 0, _T("宋体"));
    outtextxy(Width / 2 - 90, 80, "YOU WIN!");
    settextstyle(15, 0, _T("宋体"));

    if (pick_quit == 1){       // quit菜单
        if (sound_switch == 1){
            mciSendString("open bm_music\\pass_button.mp3 alias pass", NULL, 0, NULL);//鼠标经过按键的声音
            mciSendString("play pass", NULL, 0, NULL);
        }
        putimage (227 + 110, 450, &img_quit_cover,NOTSRCERASE);               //特效
        putimage (227 + 110 + a, 450 + b, &img_quit_pick,SRCINVERT);
    }
    else{
        putimage (227 + 110, 450, &img_quit_cover,NOTSRCERASE);
        putimage (227 + 110, 450, &img_quit,SRCINVERT);
    }

    if (pick_backtomenu == 1){
        if (sound_switch == 1){
            mciSendString("open bm_music\\pass_button.mp3 alias pass", NULL, 0, NULL);//鼠标经过按键的声音
            mciSendString("play pass", NULL, 0, NULL);
        }
        putimage (207, 380, &img_backtomenu_cover,NOTSRCERASE);               //特效
        putimage (207 + a, 380 + b, &img_backtomenu_pick,SRCINVERT);
    }
    else{
        putimage (207, 380, &img_backtomenu_cover,NOTSRCERASE);
        putimage (207, 380, &img_backtomenu,SRCINVERT);
    }

    if (n == 1)
    {
        putimage (450, High / 2 - 28, &img_0_cover, NOTSRCERASE);
        putimage (450, High / 2 - 28, &img_0, SRCINVERT);
    }

    for (i = 0; i < n; i ++)   //显示分数
    {
        putimage (750 - i * 48 - (9 - n) * 45, High / 2 - 28, &img_num_cover[i], NOTSRCERASE);
        putimage (750 - i * 48 - (9 - n) * 45, High / 2 - 28, &img_num[i], SRCINVERT);
    }
    FlushBatchDraw();
}

void pvp_win_user()
{
    while (MouseHit())
    {
        m=GetMouseMsg();
        if (m.x >= 227 + 110 && m.x <= 227 + 110 + 120 && m.y >= 450 && m.y <= 450 + 39)    // 点击quit
        {
            pick_quit= 1;
            if (m.uMsg == WM_LBUTTONDOWN)
            {
                gameover_quit();   //  退出游戏
            }

        }
        else{
            pick_quit = 0;
        }

        if (m.x >= 207 && m.x <= 207 + 410 && m.y >= 380 && m.y <= 380 + 39)   // 点击backtomenu
        {
            pick_backtomenu = 1;
            if (m.uMsg == WM_LBUTTONDOWN)
            {
                little_loading();
                pvp = 0;
                gameover_back_to_menu();
            }
        }
        else{
            pick_backtomenu = 0;
        }
    }
}

void pvp_win()
{
    int n;
    if (enemy_hp < 0){
        mciSendString("open bm_music\\ko.mp3 alias ko", NULL, 0, NULL);//ko音效
        mciSendString("play ko", NULL, 0, NULL);
        Sleep(2000);
        mciSendString("close ko", NULL, 0, NULL);
        n = gameover_score_pic();
        while(1)
        {
            pvp_win_show(n);
            pvp_win_user();
        }
    }
}
/*----------------------------------------------------------------------------------------*/
void pvp_update_enemy_gun()
{
    if (enemy_direction == 'a'){           //调整枪的朝向
        img_enemy_gun = img_enemy_gun_a;
        img_enemy_gun_cover = img_enemy_gun_a_cover;
    }
    else if (enemy_direction == 'd'){
        img_enemy_gun = img_enemy_gun_d;
        img_enemy_gun_cover = img_enemy_gun_d_cover;
    }
    else if (enemy_direction == 's'){
        img_enemy_gun = img_enemy_gun_s;
        img_enemy_gun_cover = img_enemy_gun_s_cover;
    }
    else if (enemy_direction == 'w'){
        img_enemy_gun = img_enemy_gun_w;
        img_enemy_gun_cover = img_enemy_gun_w_cover;
    }

    if (enemy_gun_model == 3){   // 加载shotgun图片
        img_enemy_gun_a = img_shotgun_a;
        img_enemy_gun_a_cover = img_shotgun_a_cover;
        img_enemy_gun_d = img_shotgun_d;
        img_enemy_gun_d_cover = img_shotgun_d_cover;
        img_enemy_gun_s = img_shotgun_s;
        img_enemy_gun_s_cover = img_shotgun_s_cover;
        img_enemy_gun_w = img_shotgun_w;
        img_enemy_gun_w_cover = img_shotgun_w_cover;
        enemy_fire_len = shotgun.fire_length;
    }
    else if (enemy_gun_model == 2){ // 加载uzi
        img_enemy_gun_a = img_uzi_a;
        img_enemy_gun_a_cover = img_uzi_a_cover;
        img_enemy_gun_d = img_uzi_d;
        img_enemy_gun_d_cover = img_uzi_d_cover;
        img_enemy_gun_s = img_uzi_s;
        img_enemy_gun_s_cover = img_uzi_s_cover;
        img_enemy_gun_w = img_uzi_w;
        img_enemy_gun_w_cover = img_uzi_w_cover;
        enemy_fire_len = uzi.fire_length;
    }
    else if (enemy_gun_model == 1){ // 加载handgun
        img_enemy_gun_a = img_handgun_a;
        img_enemy_gun_a_cover = img_handgun_a_cover;
        img_enemy_gun_d = img_handgun_d;
        img_enemy_gun_d_cover = img_handgun_d_cover;
        img_enemy_gun_s = img_handgun_s;
        img_enemy_gun_s_cover = img_handgun_s_cover;
        img_enemy_gun_w = img_handgun_w;
        img_enemy_gun_w_cover = img_handgun_w_cover;
        enemy_fire_len = handgun.fire_length;
    }

}

void pvp_update_enemy()
{
    if (enemy_direction == 'w')
    {
        img_enemy = img_enemy_w;
        img_enemy_cover = img_enemy_w_cover;
    }
    else if (enemy_direction == 'a')
    {
        img_enemy = img_enemy_a;
        img_enemy_cover = img_enemy_a_cover;
    }
    else if (enemy_direction == 's')
    {
        img_enemy = img_enemy_s;
        img_enemy_cover = img_enemy_s_cover;
    }
    else if (enemy_direction == 'd')
    {
        img_enemy = img_enemy_d;
        img_enemy_cover = img_enemy_d_cover;
    }
    pvp_update_enemy_gun();
}

void pvp_gameover()
{
    int n;
    if (player_hp <= 0){           // 生命值为0
        mciSendString("open bm_music\\ko.mp3 alias ko", NULL, 0, NULL);//ko音效
        mciSendString("play ko", NULL, 0, NULL);
        Sleep(2000);
        mciSendString("close ko", NULL, 0, NULL);
        gameover_rank();
        n = gameover_score_pic();
        while (1){
            gameover_show(n);
            gameover_user();
        }
    }
}

void pvp_update()
{
    hold_map();
    update_wall();
    update_walk();
    pvp_update_enemy();
    pvp_touch();
    pvp_hit();
    change_gun();
    update_gun();
    update_redbox();
    pvp_win();
    pvp_gameover();
}

void pvp_user()  // pvp中玩家位置采用绝对位置
{
    if ((GetAsyncKeyState(0x41) & 0x8000)){ //a
        player_x--;
        player_direction = 'a';
        player_walk++;
    }
    if ((GetAsyncKeyState(0x44) & 0x8000)){ //d
        player_x++;
        player_direction = 'd';
        player_walk++;
    }
    if (GetAsyncKeyState(0x57) & 0x8000 ){  // w
        player_y--;
        player_direction = 'w';
        player_walk++;
    }
    if ((GetAsyncKeyState(0x53) & 0x8000)){ //s
        player_y++;
        player_direction = 's';
        player_walk++;
    }

    if ((GetAsyncKeyState(0x4A) & 0x8000)){   // 按j开火
        if (gun_present.gun_stop >= gun_present.gun_speed && gun_present.bullet > 0){
            fire = 1;
            gun_present.bullet --;
            hit = 0;
        }
    }

    if ((GetAsyncKeyState(0x31) & 0x8000)){   // 按1换枪
        gun_model_old = gun_model;   // 记录前一把枪
        gun_model = 1;
        gun_change = 1;
    }

    if ((GetAsyncKeyState(0x32) & 0x8000)){   // 按2换枪
        gun_model_old = gun_model;
        gun_model = 2;
        gun_change = 1;
    }

    if ((GetAsyncKeyState(0x33) & 0x8000)){   // 按3换枪
        gun_model_old = gun_model;
        gun_model = 3;
        gun_change = 1;
    }

}

void play_pvp()
{
    while (1)
    {
        pvp_user();
        pvp_update();
        pvp_show();
    }
}

/*===============================================================================================*/
/*============================================主程序============================================*/
int main()
{
    startup();
    little_loading();
    menu();
    if (pvp == 0){
        play();
    }
    else if (pvp == 1){
        play_pvp();
    }
    return 0;
}
