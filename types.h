typedef struct state
{
	int playerX;
	int playerY;
	int playerHP;	//o que vai contar a hp do jogador, podera ser usado depois para o sistema de vida, com itens para curar e outros para aumentar a vida
	int playerMAXHP;	//o que vai contar o maximo valor possivel para a vida que aumenta com o item de aumentar a vida, para prevenir que "cure" para mais vida do que temdesbloqueado
	int playerDMG; 	//base do jogador que começa em 1 e pode aumentar com itens
} STATE;

typedef struct mob
{
	char mobtype;
	int mobX;
	int mobY;
	int mobHP;
	int mobDMG;
    int mobHITS;

} MOBS;