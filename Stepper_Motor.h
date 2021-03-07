#define COUNTS_PER_MS 8900
#define CW 1
#define CCW 2
#define FS 3
#define HS 4
#define BitMask (BIT_7 | BIT_8 | BIT_9 | BIT_10)
#define T1_PRESCALE 1
#define TOGGLES_PER_SEC 1000
#define T1_TICK (FPB/T1_PRESCALE/TOGGLES_PER_SEC)

void initialize_system(void);
int read_buttons(void);
int decode_buttons(void);
int stepper_state_machine(unsigned int dir, unsigned int mode);
void output_to_stepper_motor(unsigned int sm_code);
void Timer1_delay(int *step_delay, int *button_delay);
void sw_msDelay(unsigned int mS);