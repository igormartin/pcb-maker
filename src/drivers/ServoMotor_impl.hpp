/**
 * Constructor
 *
 * @param pwm     Number of pwm
 * @param channel Pwm channel
 */
template <uint16_t FREQUENCY, uint16_t MIN_PULSE, uint16_t MAX_PULSE, uint16_t MAX_DEGREE>
ServoMotor<FREQUENCY, MIN_PULSE, MAX_PULSE, MAX_DEGREE>::ServoMotor(uint8_t pwm, uint8_t channel)
{
    static servo_t device;

    this->device = &device;
    this->pwm = PWM_DEV(pwm);
    this->pwmChannel = channel;
}

/**
 * Setup pwm frequency and servo positions
 */
template <uint16_t FREQUENCY, uint16_t MIN_PULSE, uint16_t MAX_PULSE, uint16_t MAX_DEGREE>
bool ServoMotor<FREQUENCY, MIN_PULSE, MAX_PULSE, MAX_DEGREE>::init()
{
    servo_init(this->device, this->pwm, this->pwmChannel, MIN_PULSE, MAX_PULSE);

    // Update frequency
    this->device->scale_nom = pwm_init(pwm, PWM_LEFT, FREQUENCY, RESOLUTION);
    this->device->scale_den = FREQUENCY;

    return true;
}

/**
 * Rotate the servo to a specified degree
 *
 * @param degree The degree, that a servo must be rotated
 */
template <uint16_t FREQUENCY, uint16_t MIN_PULSE, uint16_t MAX_PULSE, uint16_t MAX_DEGREE>
void ServoMotor<FREQUENCY, MIN_PULSE, MAX_PULSE, MAX_DEGREE>::setDegree(uint16_t degree)
{
    uint16_t position = (uint16_t)(MIN_PULSE + (ONE_DEGREE * degree));

    printf("Servo degree: %d\n", degree);
    printf("Servo position: %d\n", position);

    servo_set(this->device, position);
}
