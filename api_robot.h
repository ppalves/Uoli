/**************************************************************** 
 * Description: Uoli Control Application Programming Interface.
 *
 * Authors: Edson Borin (edson@ic.unicamp.br)
 *          AntÃ´nio GuimarÃ£es ()
 *
 * Date: 2019
 ***************************************************************/
#ifndef API_ROBOT_H
#define API_ROBOT_H

/*
 * Struct to represent 3-dimensional vectors. 
 */
typedef struct 
{
  int x;
  int y;
  int z;
} Vector3;

/**************************************************************/
/* List of Friends locations and dangerous locations          */
/**************************************************************/

Vector3 friends_locations[5] = {
  {.x = 715, .y = 105, .z = -40},
  {.x = 613, .y = 105, .z = -24},
  {.x = 447, .y = 106, .z = 158},
  {.x = 507, .y = 105, .z = 264},
  {.x = 596, .y = 105, .z = 402}
};

Vector3 dangerous_locations[5] = {
  {.x = 475, .y = 104, .z = 193},
  {.x = 526, .y = 105, .z = 225},
  {.x = 487, .y = 105, .z = -10},
  {.x = 685, .y = 105, .z = -21},
  {.x = 421, .y = 105, .z = 116}
};


/**************************************************************/
/* Engines                                                    */
/**************************************************************/

/* 
 * Sets both engines torque. The torque value must be between -100 and 100.
 * Parameter: 
 *   engine_1: Engine 1 torque
 *   engine_2: Engine 2 torque
 * Returns:
 *   -1 in case one or more values are out of range
 *    0 in case both values are in range
 */
int set_torque(int engine_1, int engine_2);

/* 
 * Sets engine torque. Engine ID 0/1 identifies the left/right engine.
 * The torque value must be between -100 and 100.
 * Parameter: 
 *   engine_id: Engine ID
 *   torque: Engine torque
 * Returns:
 *   -1 in case the torque value is invalid (out of range)
 *   -2 in case the engine_id is invalid
 *    0 in case both values are valid
 */
int set_engine_torque(int engine_id, int torque);

/* 
 * Sets the angle of three Servo motors that control the robot head. 
 *   Servo ID 0/1/2 identifies the Base/Mid/Top servos.
 * Parameter: 
 *   servo_id: Servo ID 
 *   angle: Servo Angle 
 * Returns:
 *   -1 in case the servo id is invalid
 *   -2 in case the servo angle is invalid
 *    0 in case the servo id and the angle is valid
 */
int set_head_servo(int servo_id, int angle);

/**************************************************************/
/* Sensors                                                    */
/**************************************************************/

/* 
 * Reads distance from ultrasonic sensor.
 * Parameter: 
 *   none
 * Returns:
 *   distance of nearest object within the detection range, in centimeters.
 */
unsigned short get_us_distance();

/* 
 * Reads current global position using the GPS device.
 * Parameter: 
 *   pos: pointer to structure to be filled with the GPS coordinates.
 * Returns:
 *   void
 */
void get_current_GPS_position(Vector3* pos);

/* 
 * Reads global rotation from the gyroscope device .
 * Parameter: 
 *   pos: pointer to structure to be filled with the Euler angles indicated by the gyroscope.
 * Returns:
 *   void
 */
void get_gyro_angles(Vector3* angles);

/**************************************************************/
/* Timer                                                      */
/**************************************************************/

/* 
 * Reads the system time. 
 * Parameter:
 *   * t: pointer to a variable that will receive the system time.
 * Returns:
 *   The system time (in milliseconds)
 */
unsigned int get_time();

/* 
 * Sets the system time.
 * Parameter: 
 *   t: the new system time.
 * Returns:
 *   void
 */
void set_time(unsigned int t);

/**************************************************************/
/* UART                                                       */
/**************************************************************/

/* 
 * Writes a string to the UART. Uses the syscall write with file 
 * descriptor 1 to instruct the SOUL to write the string to the UART.
 * Parameter:
 *   * s: pointer to the string.
 * Returns:
 *   void
 */
void puts(const char*);

#endif // API_ROBOT_H



