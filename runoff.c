#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // TODO //need to add candidate_count and preferences[i][j]
    for(int i = 0; i < candidate_count; i++) //if voter less than candidate, vote++
    {
        if(strcmp(candidates[i].name, name) == 0) //use strcmp to compare names in array with names input during vote. if the same..
        {
            preferences[voter][rank] = i; //use preferences to store the voter and their rankings.
            return true; //return command true
        }
    }
    return false; //no match, false
}

// Tabulate votes for non-eliminated candidates
void tabulate(void) //this one stumped me the longest
{
    // TODO
    for(int i = 0; i < voter_count; i++) //for i less than voter count tabulating votes.. continue
    {
        for(int j = 0; j < candidate_count; j++) //if second voter less than candidate count. second voter++
        {
            int candidate_index = preferences[i][j]; //int candidate index equals preferences
            if(candidates[candidate_index].eliminated == false) //if candidates.eliminated is the same value as false. meaning not eliminated.
            {
                candidates[candidate_index].votes++; //vote++
                break; //break the cycle after second vote
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // TODO
    float majority = (float)voter_count / 2; //need to create a new number 'majority', same as voter count divided by 2.
    for(int i = 0; i < candidate_count; i++) //initiate
    {
        if((int)candidates[i].votes > majority) //if candidates votes are more than majority. the half of voter count..
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // TODO
    int min_vote = MAX_VOTERS; //min vote can only be the same as max voter number.
    for(int i = 0; i < candidate_count; i++) //initiate
    {
        if(candidates[i].eliminated == false && candidates[i].votes < min_vote) //if not eliminated, and has votes less than minimum vote count
        {
            min_vote = candidates[i].votes; //then we found our minimum vote. now equal to candidate votes.
        }
    }
    return min_vote;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // TODO
    for(int i = 0; i < candidate_count; i++)//; //initiate
    {
        if(candidates[i].eliminated == false && candidates[i].votes != min) //if candidates elimination status and votes are not equal to min
        {
            return false; //cannot possibly be tie
        }
    }
    return true;
}

// Eliminate the candidate (or candidiates) in last place
void eliminate(int min)
{
    // TODO
    for(int i = 0; i < candidate_count; i++)//;
    {
        if(candidates[i].votes == min) //if candidate vote is now equivalent to min
        {
            candidates[i].eliminated = true; //candidate eliminated
        }
    }
    return;
}
