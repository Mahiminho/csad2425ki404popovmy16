#include <Arduino.h>
#include <vector>
#include <algorithm>
#include <map>
#include "../.pio/libdeps/d1_wroom_02/tinyxml2/tinyxml2.h"

using namespace tinyxml2;

// Global variables and neural network parameters
/**
 * @brief Memory of the AI that stores the last three moves.
 */
std::vector<int> aiMemory = { 2, 1, 3 };

/**
 * @brief Input weights for the neural network (3 inputs and 8 neurons in hidden layer).
 */
float inputWeights[3][8] = {
    { 0.237058, -0.290494, 0.274185, 0.002906, -0.130077, 0.604973, 0.687958, 0.113789 },
    { 0.486323, -0.628314, 0.194522, 0.172590, 0.763265, -0.167007, 1.251667, 0.067055 },
    { 0.120458, -0.396984, 1.712663, 1.075813, 0.168848, -0.612591, -1.263713, 1.474092 },
};

/**
 * @brief Hidden layer weights (8 neurons in hidden layer and 3 outputs).
 */
float hiddenWeights[8][3] = {
    { 0.469825, -0.215859, -0.548794 },
    { 0.285107, -0.448687, -0.552901 },
    { 0.656990, -1.276879, 0.272193 },
    { 1.293255, -1.585455, -1.156214 },
    { -0.498069, 0.174363, 0.186442 },
    { -0.631017, 1.110896, 0.074588 },
    { -0.578895, 0.978867, 0.364652 },
    { 1.034574, -1.109686, 0.851510 },
};

/**
 * @brief Hidden layer biases.
 */
float hiddenBias[8] = {-0.376692, 0.000000, -0.370774, -0.863258, 0.907450, 0.770704, 1.312167, -0.110566};

/**
 * @brief Output biases.
 */
float outputBias[3] = {-0.725252, 0.632030, 0.405089};

/**
 * @brief Computes the ReLU activation function.
 * 
 * @param x Input value.
 * @return float Result of the ReLU function.
 */
float Relu(float x);

/**
 * @brief Applies the SoftMax function to an array of values.
 * 
 * @param x Array of values to be transformed.
 * @param size Size of the array.
 */
void SoftMax(float x[], int size);

/**
 * @brief Makes an AI-based smart choice for the game move.
 * 
 * This function uses a simple neural network to predict the best move based on previous history.
 * 
 * @return String The predicted move for the AI (either "1", "2", or "3").
 */
String SmartChoiceAI();

/**
 * @brief Determines the winner of a game between two players.
 * 
 * @param p1 Move of the first player.
 * @param p2 Move of the second player.
 * @return String The winner of the game ("1" for player 1, "2" for player 2, "0" for draw).
 */
String chooseWinner(String p1, String p2);

/**
 * @brief Initializes the system and sets up serial communication.
 */
void setup();

/**
 * @brief Main loop of the program that handles incoming data, parses XML, and processes game logic.
 * 
 * The loop listens for serial data, parses the incoming message as XML, processes the game logic, and updates the XML response.
 */
void loop();

/**
 * @brief Determines the winner of a game between two moves.
 * 
 * @param p1 The move of player 1.
 * @param p2 The move of player 2.
 * @return String The result of the game ("1" for player 1, "2" for player 2, "0" for a tie).
 */
String chooseWinner(String p1, String p2);

/**
 * @brief Implements the ReLU activation function.
 * 
 * @param x Input value.
 * @return float Output value after applying the ReLU function.
 */
float Relu(float x)
{
    return x > 0 ? x : 0;
}

/**
 * @brief Applies the SoftMax function to an array.
 * 
 * The SoftMax function converts a vector of values to probabilities that sum to 1.
 * 
 * @param x The input array to be transformed.
 * @param size The size of the input array.
 */
void SoftMax(float x[], int size)
{
    float max_val = x[0];
    for (int i = 1; i < size; i++)
        if (x[i] > max_val)
            max_val = x[i];
    float sum = 0;
    for (int i = 0; i < size; i++)
        sum += exp(x[i] - max_val);
    for (int i = 0; i < size; i++)
        x[i] = exp(x[i] - max_val) / sum;
}

/**
 * @brief Makes a smart move choice using a neural network.
 * 
 * The AI uses previous moves stored in `aiMemory` to predict the best next move based on its internal neural network.
 * 
 * @return String The predicted move ("1", "2", or "3").
 */
String SmartChoiceAI()
{
    float output[3];

    float hiddenLayer[8];
    for (int i = 0; i < 8; i++)
    {
        hiddenLayer[i] = hiddenBias[i];
        for (int j = 0; j < 3; j++)
        {
            hiddenLayer[i] += aiMemory[j] * inputWeights[j][i];
        }
        hiddenLayer[i] = Relu(hiddenLayer[i]);
    }

    for (int i = 0; i < 3; i++)
    {
        output[i] = outputBias[i];
        for (int j = 0; j < 8; j++)
        {
            output[i] += hiddenLayer[j] * hiddenWeights[j][i];
        }
    }

    SoftMax(output, 3);

    int best_move = 0;
    if (output[1] > output[best_move])
        best_move = 1;
    if (output[2] > output[best_move])
        best_move = 2;

    return String(best_move + 1);
}
