#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SIM_TIME 1370

// Declaring the variables

// miscellaneous variables
bool button_pressed = true;
bool engine_status;

// hardware variables
bool speed_sensor_state = true;
bool brake_sensor_state = true;
bool eng_temp_sensor_state = true;
bool battery_sensor_state = true;
bool hood_sensor_state = true;
bool trunk_sensor_state = true;
bool door_sensor_state = true;
bool seatbelt_sensor_state = true;
bool air_cond_sensor_state = true;

// dynamics variables
float mpha[SIM_TIME];
bool lim_clutch_dis[SIM_TIME];
int time[SIM_TIME];
float *p_mpha = mpha;
bool *p_lim_clutch_dis = lim_clutch_dis;
int *p_time = time;

// power variables
float fc_tmp[SIM_TIME];
float SOC[SIM_TIME];
float *p_fc_tmp = fc_tmp;
float *p_SOC = SOC;

// safety variables
bool hood_status = true;
bool trunk_status = true;
bool door_status = true;
bool seatbelt_status = true;

// wellbeing variables
unsigned char air_cond_speed = 3;

void loadVariablesFromFile(float *p_mpha, bool *p_lim_clutch_dis, int *p_time, float *p_fc_tmp, float *p_SOC)
{
    FILE *pFile;
    int newlineCount = 0;
    long fileSize, bytesRead = 0, bytesCount;
    char *buffer, ch;
    size_t result;

    pFile = fopen("variables.txt", "r");
    if (pFile == NULL) {fputs ("File error", stderr); exit (1);}

    // count newline characters
    do {
        ch = fgetc (pFile);
        if (ch == '\n') 
        {
            newlineCount++;
        }
    } while (ch != EOF);

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    fileSize = ftell(pFile);
    rewind(pFile);

    // allocate memory to contain the whole file:
    buffer = (char*) malloc(sizeof(char) * fileSize);
    if (buffer == NULL) {fputs("Memory error", stderr); exit (2);}

    // copy the file into the buffer:
    result = fread(buffer, sizeof(char), fileSize, pFile);
    if (result != fileSize - newlineCount) {fputs("Reading error", stderr); exit (3);}

    for (int i = 0; i < SIM_TIME; i++)
    {
        sscanf(buffer + bytesRead, "%f %d %d %f %f\n%n", p_mpha, p_lim_clutch_dis, p_time, p_fc_tmp, p_SOC, &bytesCount);
        p_mpha++;
        p_lim_clutch_dis++;
        p_time++;
        p_fc_tmp++;
        p_SOC++;
        bytesRead += bytesCount;
    }

    // terminate
    fclose(pFile);
    free(buffer);
}
