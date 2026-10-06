#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_TEAMS    16
#define MAX_MATCHES  15      
#define NAME_LEN     30
#define MAX_ROUNDS    4      

typedef struct
{
    int  id;
    char name[NAME_LEN];
    char captain[NAME_LEN];
    int  wins;             
    int  eliminated;       
    int  roundEliminated;  
} Team;

Team teams[MAX_TEAMS];
int  teamCount      = 0;
int  tournamentStarted = 0;
typedef struct Player
{
    int           id;
    char          name[NAME_LEN];
    int           teamId;
    struct Player *next;
} Player;

Player *playerHead      = NULL;
int     playerIdCounter = 1;

typedef struct
{
    int  matchId;
    int  round;       
    int  team1;        
    int  team2;
    int  score1;
    int  score2;
    int  winner;      
    int  penalties;   
    int  penScore1;
    int  penScore2;
} KOMatch;

KOMatch matchQueue[MAX_MATCHES];
int     qFront = 0;
int     qRear  = 0;

KOMatch matchStack[MAX_MATCHES];
int     top = -1;

int graph[MAX_TEAMS][MAX_TEAMS];

int currentRound    = 0;
int totalRounds     = 0;
int matchIdCounter  = 1;
int champion        = -1; 
static const char *roundName(int r, int totalR)
{
    int diff = totalR - r;
    if (diff == 0) return "FINAL";
    if (diff == 1) return "SEMI-FINAL";
    if (diff == 2) return "QUARTER-FINAL";
    return "ROUND";
}

void readString(char *buf, int maxLen)
{
    int i = 0, c;
    while ((c = getchar()) != '\n' && c != EOF);
    while (i < maxLen - 1)
    {
        c = getchar();
        if (c == '\n' || c == EOF) break;
        buf[i++] = (char)c;
    }
    buf[i] = '\0';
}

int isPowerOf2(int n)
{
    return (n > 1) && ((n & (n - 1)) == 0);
}

void addTeam()
{
    if (tournamentStarted)
    {
        printf("\nCannot add teams after the tournament has started!\n");
        return;
    }
    if (teamCount >= MAX_TEAMS)
    {
        printf("\nTeam limit reached! (%d teams max)\n", MAX_TEAMS);
        return;
    }

    Team *t        = &teams[teamCount];
    t->id          = teamCount + 1;
    t->wins        = 0;
    t->eliminated  = 0;
    t->roundEliminated = 0;

    printf("\nEnter Team Name    : ");
    readString(t->name, NAME_LEN);

    printf("Enter Captain Name : ");
    readString(t->captain, NAME_LEN);

    printf("Team added! ID = %d\n", t->id);
    teamCount++;
}

static int cmpIdx(const void *a, const void *b)
{
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return strcmp(teams[ia].name, teams[ib].name);
}

int binarySearchTeam(const char *name)
{
    if (teamCount == 0) return -1;

    int sorted[MAX_TEAMS];
    int i;
    for (i = 0; i < teamCount; i++) sorted[i] = i;
    qsort(sorted, teamCount, sizeof(int), cmpIdx);

    int lo = 0, hi = teamCount - 1;
    while (lo <= hi)
    {
        int mid = (lo + hi) / 2;
        int cmp = strcmp(teams[sorted[mid]].name, name);
        if (cmp == 0)     return sorted[mid];
        else if (cmp < 0) lo = mid + 1;
        else              hi = mid - 1;
    }
    return -1;
}

void searchTeam()
{
    char name[NAME_LEN];
    printf("\nEnter Team Name to search: ");
    readString(name, NAME_LEN);

    int idx = binarySearchTeam(name);
    if (idx == -1)
    {
        printf("Team '%s' not found.\n", name);
        return;
    }

    Team *t = &teams[idx];
    printf("\n--- Team Found (Binary Search) ---\n");
    printf("ID        : %d\n",  t->id);
    printf("Name      : %s\n",  t->name);
    printf("Wins   : %d\n",  t->wins);
    if (t->eliminated)
        printf("Status    : ELIMINATED (Round %d)\n", t->roundEliminated);
    else if (tournamentStarted)
        printf("Status    : ACTIVE\n");
    else
        printf("Status    : Registered\n");
}

void addPlayer()
{
    Player *p = (Player *)malloc(sizeof(Player));
    if (p == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    p->id = playerIdCounter++;

    printf("\nEnter Player Name : ");
    readString(p->name, NAME_LEN);

    printf("Enter Team ID     : ");
    scanf("%d", &p->teamId);

    if (p->teamId < 1 || p->teamId > teamCount)
    {
        printf("Invalid Team ID!\n");
        free(p);
        playerIdCounter--;
        return;
    }

    p->next    = playerHead;
    playerHead = p;

    printf("Player '%s' added! ID = %d\n", p->name, p->id);
}

void deletePlayer()
{
    char name[NAME_LEN];
    printf("\nEnter Player Name to delete: ");
    readString(name, NAME_LEN);

    Player *cur  = playerHead;
    Player *prev = NULL;

    while (cur != NULL)
    {
        if (strcmp(cur->name, name) == 0)
        {
            if (prev == NULL)
                playerHead = cur->next;
            else
                prev->next = cur->next;

            printf("Player '%s' (ID=%d) deleted.\n", cur->name, cur->id);
            free(cur);
            return;
        }
        prev = cur;
        cur  = cur->next;
    }

    printf("Player '%s' not found.\n", name);
}

void startTournament()
{
    if (tournamentStarted)
    {
        printf("\nTournament has already started!\n");
        return;
    }
    if (teamCount < 2)
    {
        printf("\nNeed at least 2 teams to start!\n");
        return;
    }
    if (!isPowerOf2(teamCount))
    {
        int next = 1;
        while (next <= teamCount) next <<= 1;
        printf("\nTeam count must be a power of 2 (2, 4, 8, 16).\n");
        printf("Currently: %d teams. Add %d more team(s) or remove some.\n",
               teamCount, next - teamCount);
        return;
    }

    tournamentStarted = 1;
    currentRound      = 1;
    totalRounds       = 0;
    {
        int n = teamCount;
        while (n > 1) { totalRounds++; n >>= 1; }
    }
    qFront = qRear = 0;
    top            = -1;
    memset(graph, 0, sizeof(graph));

    printf("\n");
    printf("*****************************************************\n");
    printf("   KNOCKOUT TOURNAMENT STARTED!  (%d Teams)\n", teamCount);
    printf("   Total Rounds : %d\n", totalRounds);
    printf("*****************************************************\n");

    int i;
    printf("\n--- Round 1 Bracket ---\n");
    for (i = 0; i < teamCount; i += 2)
    {
        KOMatch m;
        m.matchId   = matchIdCounter++;
        m.round     = 1;
        m.team1     = i + 1;
        m.team2     = i + 2;
        m.score1    = 0;
        m.score2    = 0;
        m.winner    = 0;
        m.penalties = 0;
        m.penScore1 = 0;
        m.penScore2 = 0;

        matchQueue[qRear++] = m;

        graph[i][i + 1] = 1;
        graph[i + 1][i] = 1;

        printf("  Match %d : %s  vs  %s\n",
               m.matchId,
               teams[m.team1 - 1].name,
               teams[m.team2 - 1].name);
    }

    printf("\nRound 1 matches queued. Use option 9 to play them!\n");
}

static int collectRoundWinners(int round, int *buf)
{
    int cnt = 0, i;
    for (i = 0; i <= top; i++)
        if (matchStack[i].round == round)
            buf[cnt++] = matchStack[i].winner;
    return cnt;
}

static void queueNextRound(int *winnerIds, int cnt)
{
    int i;
    currentRound++;
    printf("\n");
    printf("=============================================\n");
    printf("  ROUND %d  -  %s\n",
           currentRound, roundName(currentRound, totalRounds));
    printf("=============================================\n");

    for (i = 0; i < cnt; i += 2)
    {
        KOMatch m;
        m.matchId   = matchIdCounter++;
        m.round     = currentRound;
        m.team1     = winnerIds[i];
        m.team2     = winnerIds[i + 1];
        m.score1    = 0;
        m.score2    = 0;
        m.winner    = 0;
        m.penalties = 0;
        m.penScore1 = 0;
        m.penScore2 = 0;

        matchQueue[qRear++] = m;

        graph[m.team1 - 1][m.team2 - 1] = 1;
        graph[m.team2 - 1][m.team1 - 1] = 1;

        printf("  Match %d : %s  vs  %s\n",
               m.matchId,
               teams[m.team1 - 1].name,
               teams[m.team2 - 1].name);
    }
}

void completeMatch()
{
    if (!tournamentStarted)
    {
        printf("\nStart the tournament first! (Option 9)\n");
        return;
    }
    if (champion != -1)
    {
        printf("\nTournament is over! Champion: %s\n",
               teams[champion].name);
        return;
    }
    if (qFront >= qRear)
    {
        printf("\nNo upcoming matches in the queue!\n");
        return;
    }

    KOMatch match = matchQueue[qFront++];
    Team   *t1    = &teams[match.team1 - 1];
    Team   *t2    = &teams[match.team2 - 1];

    printf("\n=============================================\n");
    printf("  %s  -  Match %d\n",
           roundName(match.round, totalRounds), match.matchId);
    printf("=============================================\n");
    printf("  %s  vs  %s\n", t1->name, t2->name);
    printf("---------------------------------------------\n");

    printf("Score for %-15s : ", t1->name);
    scanf("%d", &match.score1);
    printf("Score for %-15s : ", t2->name);
    scanf("%d", &match.score2);

    if (match.score1 == match.score2)
    {
        printf("\nScores are level! Penalty Shootout!\n");
        match.penalties = 1;

        do {
            printf("Penalty score for %-15s : ", t1->name);
            scanf("%d", &match.penScore1);
            printf("Penalty score for %-15s : ", t2->name);
            scanf("%d", &match.penScore2);

            if (match.penScore1 == match.penScore2)
                printf("Still level! Enter penalty scores again.\n");
        } while (match.penScore1 == match.penScore2);

        match.winner = (match.penScore1 > match.penScore2)
                       ? match.team1 : match.team2;

        printf("\n%s wins on penalties (%d-%d)!\n",
               teams[match.winner - 1].name,
               match.penScore1, match.penScore2);
    }
    else if (match.score1 > match.score2)
    {
        match.winner = match.team1;
        printf("\n%s wins! (%d-%d)\n", t1->name, match.score1, match.score2);
    }
    else
    {
        match.winner = match.team2;
        printf("\n%s wins! (%d-%d)\n", t2->name, match.score1, match.score2);
    }

    int loser = (match.winner == match.team1) ? match.team2 : match.team1;
    teams[match.winner - 1].wins++;
    teams[loser - 1].eliminated      = 1;
    teams[loser - 1].roundEliminated = match.round;

    printf("  >> %s is ELIMINATED.\n", teams[loser - 1].name);

    if (top < MAX_MATCHES - 1)
        matchStack[++top] = match;

    int roundMatchesTotal = teamCount >> match.round;
    int roundMatchesDone  = 0;
    int i;
    for (i = 0; i <= top; i++)
        if (matchStack[i].round == match.round)
            roundMatchesDone++;

    if (roundMatchesDone == roundMatchesTotal)
    {
        if (match.round == totalRounds)
        {
            champion = match.winner - 1;
            printf("\n");
            printf("*****************************************************\n");
            printf("  TOURNAMENT CHAMPION : %s\n", teams[champion].name);
            printf("  Captain             : %s\n", teams[champion].captain);
            printf("*****************************************************\n");
        }
        else
        {
            int winners[MAX_TEAMS];
            int wCnt = collectRoundWinners(match.round, winners);
            if (wCnt >= 2)
                queueNextRound(winners, wCnt);
        }
    }
    else
    {
        int remaining = roundMatchesTotal - roundMatchesDone;
        printf("  (%d match(es) remaining in this round)\n", remaining);
    }
}

void displayUpcomingMatches()
{
    if (!tournamentStarted)
    {
        printf("\nTournament has not started yet.\n");
        return;
    }
    if (qFront >= qRear)
    {
        if (champion != -1)
            printf("\nTournament complete. Champion: %s\n",
                   teams[champion].name);
        else
            printf("\nNo upcoming matches scheduled yet.\n");
        return;
    }

    printf("\n===== UPCOMING MATCHES =====\n");
    int i;
    for (i = qFront; i < qRear; i++)
    {
        KOMatch *m = &matchQueue[i];
        printf("  Match %-2d | %-14s | %-15s  vs  %-15s\n",
               m->matchId,
               roundName(m->round, totalRounds),
               teams[m->team1 - 1].name,
               teams[m->team2 - 1].name);
    }
}

void viewMatchHistory()
{
    if (top == -1)
    {
        printf("\nNo completed matches yet.\n");
        return;
    }

    printf("\n===== MATCH HISTORY (Latest First) =====\n");
    int i;
    for (i = top; i >= 0; i--)
    {
        KOMatch *m = &matchStack[i];
        printf("%2d. [%-14s] Match %-2d | %-15s %d-%d  %-15s",
               top - i + 1,
               roundName(m->round, totalRounds),
               m->matchId,
               teams[m->team1 - 1].name,
               m->score1, m->score2,
               teams[m->team2 - 1].name);

        printf("  => %s wins", teams[m->winner - 1].name);
        if (m->penalties)
            printf(" (pen %d-%d)", m->penScore1, m->penScore2);
        printf("\n");
    }
}

void displayBracket()
{
    if (!tournamentStarted)
    {
        printf("\nTournament has not started yet.\n");
        return;
    }

    printf("\n");
    printf("====================================================================\n");
    printf("                     KNOCKOUT BRACKET\n");
    printf("====================================================================\n");

    int r, i;
    for (r = 1; r <= currentRound; r++)
    {
        printf("\n  --- %s (Round %d) ---\n", roundName(r, totalRounds), r);
        printf("  %-4s | %-15s | Score | %-15s | Winner\n",
               "Mtch", "Team 1", "Team 2");
        printf("  ------------------------------------------------------------------\n");

        for (i = 0; i <= top; i++)
        {
            KOMatch *m = &matchStack[i];
            if (m->round != r) continue;

            char result[60];
            if (m->penalties)
                sprintf(result, "%s (pen %d-%d)",
                        teams[m->winner - 1].name,
                        m->penScore1, m->penScore2);
            else
                sprintf(result, "%s", teams[m->winner - 1].name);

            printf("  %-4d | %-15s | %2d-%2d  | %-15s | %s\n",
                   m->matchId,
                   teams[m->team1 - 1].name,
                   m->score1, m->score2,
                   teams[m->team2 - 1].name,
                   result);
        }

        for (i = qFront; i < qRear; i++)
        {
            KOMatch *m = &matchQueue[i];
            if (m->round != r) continue;
            printf("  %-4d | %-15s | Pend. | %-15s | (not played)\n",
                   m->matchId,
                   teams[m->team1 - 1].name,
                   teams[m->team2 - 1].name);
        }
    }

    if (champion != -1)
    {
        printf("\n");
        printf("  *** CHAMPION : %s ***\n", teams[champion].name);
    }
    printf("====================================================================\n");
}

void displayStandings()
{
    if (teamCount == 0)
    {
        printf("\nNo teams added yet.\n");
        return;
    }

    int order[MAX_TEAMS];
    int i, j, tmp;
    for (i = 0; i < teamCount; i++) order[i] = i;

    for (i = 0; i < teamCount - 1; i++)
        for (j = 0; j < teamCount - i - 1; j++)
        {
            Team *a = &teams[order[j]];
            Team *b = &teams[order[j + 1]];
            int swap = 0;

            if (a->wins < b->wins)
                swap = 1;
            else if (a->wins == b->wins &&
                     a->roundEliminated < b->roundEliminated)
                swap = 1;

            if (swap)
            {
                tmp        = order[j];
                order[j]   = order[j + 1];
                order[j+1] = tmp;
            }
        }

    printf("\n");
    printf("=================================================================\n");
    printf("     STANDINGS  (Bubble Sort: Wins / Round Reached)\n");
    printf("=================================================================\n");
    printf("  %-4s | %-15s | %-5s | Status\n",
           "Rank", "Team", "Wins");
    printf("  ---------------------------------------------------------------\n");

    for (i = 0; i < teamCount; i++)
    {
        Team *t = &teams[order[i]];
        char status[40];

        if (champion != -1 && order[i] == champion)
            sprintf(status, "CHAMPION");
        else if (t->eliminated)
            sprintf(status, "Eliminated (Rd %d)", t->roundEliminated);
        else if (tournamentStarted)
            sprintf(status, "Active");
        else
            sprintf(status, "Registered");

        printf("  %-4d | %-15s | %-15s | %-5d | %s\n",
               i + 1, t->name, t->captain, t->wins, status);
    }
    printf("=================================================================\n");
}

void displayGraph()
{
    int i, j, hasEdge;

    printf("\n===== KNOCKOUT MATCH GRAPH (Adjacency Matrix) =====\n");

    for (i = 0; i < teamCount; i++)
    {
        printf("%-15s -> ", teams[i].name);
        hasEdge = 0;
        for (j = 0; j < teamCount; j++)
        {
            if (graph[i][j] == 1)
            {
                printf("%s  ", teams[j].name);
                hasEdge = 1;
            }
        }
        if (!hasEdge) printf("(no matches played yet)");
        printf("\n");
    }
}

void displayTeams()
{
    if (teamCount == 0)
    {
        printf("\nNo teams registered yet.\n");
        return;
    }
    printf("\n===== REGISTERED TEAMS =====\n");
    printf("  %-4s | %-15s | Status\n", "ID", "Team");
    printf("  -------------------------------------------------\n");
    int i;
    for (i = 0; i < teamCount; i++)
    {
        Team *t = &teams[i];
        char status[25];
        if (champion != -1 && i == champion)
            strcpy(status, "CHAMPION");
        else if (t->eliminated)
            sprintf(status, "Eliminated (Rd %d)", t->roundEliminated);
        else if (tournamentStarted)
            strcpy(status, "Active");
        else
            strcpy(status, "Registered");
        printf("  %-4d | %-15s | %s\n",
               t->id, t->name, status);
    }
}

int main()
{
    int choice;
    memset(graph, 0, sizeof(graph));

    while (1)
    {
        printf("\n=====================================\n");
        printf("   SPORTS TOURNAMENT MANAGEMENT \n");
        printf("=====================================\n");
        if (tournamentStarted && champion == -1)
            printf("  [ROUND %d - %s IN PROGRESS]\n",
                   currentRound, roundName(currentRound, totalRounds));
        else if (champion != -1)
            printf("  [TOURNAMENT COMPLETE - CHAMPION: %s]\n",
                   teams[champion].name);
        else
            printf("  [Registration Phase - %d team(s) registered]\n", teamCount);
        printf("-------------------------------------------------------\n");
        printf(" TEAM\n");
        printf("  1.  Add Team\n");
        printf("  2.  View All Teams\n");
        printf("  3.  Search Team\n");
        printf(" PLAYER\n");
        printf("  4.  Add Player\n");
        printf("  5.  Delete Player\n");
        printf(" TOURNAMENT\n");
        printf("  6.  Start Tournament\n");
        printf("  7.  Play Next Match\n");
        printf("  8. Upcoming Matches\n");
        printf("  9. Match History\n");
        printf("  10. Knockout Bracket\n");
        printf(" ANALYTICS\n");
        printf("  11. Standings\n");
        printf("  12. Match Graph\n");
        printf("  13.  Exit\n");
        printf("=====================================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:  addTeam();                break;
            case 2:  displayTeams();           break;
            case 3:  searchTeam();             break;
            case 4:  addPlayer();              break;
            case 5:  deletePlayer();           break;
            case 6:  startTournament();        break;
            case 7:  completeMatch();          break;
            case 8: displayUpcomingMatches();  break;
            case 9: viewMatchHistory();        break;
            case 10: displayBracket();         break;
            case 11: displayStandings();       break;
            case 12: displayGraph();           break;
            case 13:
                printf("\nThank you! Goodbye!\n");
                {
                    Player *cur = playerHead;
                    while (cur != NULL)
                    {
                        Player *tmp = cur->next;
                        free(cur);
                        cur = tmp;
                    }
                }
                return 0;
            default:
                printf("\nInvalid choice! Please enter 0-15.\n");
        }
    }
    
    return 0;
}