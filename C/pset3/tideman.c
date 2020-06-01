//20 May, 2020.
//Lawrence Wong. CS50x. Pset3 - Tideman.

#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void check_cycle(int i);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    //Debugged, all is well
    for (int i = 0; i < candidate_count; i++)
    {
        //Match name with the array of candidates, assign rank to the i-th candidate
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    //Debugged, all is well
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if ((i != j) && (i < j))
            {
                preferences[ranks[i]][ranks[j]]++;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int n = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if ((i != j) && (preferences[i][j] > preferences[j][i]))
            {
                pairs[n].winner = i;
                pairs[n].loser = j;
                pair_count++, n++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    //Bubble sort the preferences
    int sortWin, sortLost;
    
    for (int i = 0; i < (candidate_count - 1); i++)
    {
        for (int j = 0; j < (candidate_count - 1); j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] < preferences[pairs[j + 1].winner][pairs[j + 1].loser])
            {
                sortWin = pairs[j + 1].winner;
                sortLost = pairs[j + 1].loser;
                pairs[j + 1].winner = pairs[j].winner;
                pairs[j + 1].loser = pairs[j].loser;
                pairs[j].winner = sortWin;
                pairs[j].loser = sortLost;
            }
        }
    }
    return;
}

void lock_pairs(void)
{
    check_cycle(0);
    return;
}

void check_cycle(int locking)
{
    if (locking == pair_count)
    {
        return;
    }
    
    //Lock each pair to initialize
    locked[pairs[locking].winner][pairs[locking].loser] = true;
    
    //Lock the next pair recursively
    check_cycle(locking + 1);
    
    //Return if there's no loop
    for (int i = 0; i < pair_count; i++)
    {
        //Return if any of the previous pairs are not locked
        if (locked[pairs[i].winner][pairs[i].loser] == false)
        {
            return;
        }
    }
    
    //Flag currently locking pairs as false if there's a loop
    locked[pairs[locking].winner][pairs[locking].loser] = false;
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int winner = pairs[0].winner;
    for (int i = 0; i < pair_count; i++)
    {
        if ((locked[pairs[i].winner][pairs[i].loser] == 1) && (winner != pairs[i].loser))
        {
            continue;
        }
        else if ((locked[pairs[i].winner][pairs[i].loser] == 1) && (winner == pairs[i].loser))
        {
            winner = pairs[i].winner;
        }
    }
    printf("%s\n", candidates[winner]);
    return;
}
