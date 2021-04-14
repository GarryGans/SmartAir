#include "Key.h"

Key::Key(byte pin[]) : AmperkaKB(pin[0], pin[1], pin[2], pin[3], pin[4])
{
}

Key::~Key()
{
}

void Key::select()
{
    if (justPressed())
    {
        switch (getNum)
        {
        case PLUS:

            break;

        case MINUS:

            break;

        case SELECT:
            switch (mode)
            {
            case MANUAL:

                switch (fanState)
                {
                case swOFF:
                    fanState = swON;
                    break;

                case swON:
                    fanState = swOFF;
                    break;

                default:
                    break;
                }

                break;

            case LIGHT:
                switch (highLight)
                {
                case TIME:
                    highLight = DATE;
                    break;

                case DATE:
                    highLight = START;
                    break;

                case START:
                    highLight = STOP;
                    break;

                case STOP:
                    highLight = TIME;
                    break;

                default:
                    break;
                }
                break;

            default:
                break;
            }

            break;

        case MODE:

            break;

        default:

            break;
        }
    }

    else if (isHold())
    {
        switch (getNum)
        {
        case PLUS:

            break;

        case MINUS:
            /* code */
            break;

        case SELECT:

            break;

        case MODE:

            switch (mode)
            {
            case AUTO:
                mode = LIGHT;
                break;

            case LIGHT:
                mode = AUTO;
                highLight = TIME;
                break;

            default:
                break;
            }

            break;

        default:
            break;
        }
    }
}



void Key::keyCommands()
{
    read();

    
}

void Key::manualSwitcher()
{
    switch (mode)
    {
    case MANUAL:

        switch (fanMode)
        {
        case HIGH_SPEED:
            fanMode = LOW_SPEED;
            break;

        case LOW_SPEED:
            fanMode = HIGH_SPEED;
            break;

        default:
            break;
        }

        break;

    default:
        break;
    }

    if (fanState == swON && !manualFlow[0])
    {
        manualFlow[0] = true;
        manualFlow[1] = false;
        fanMode = LOW_SPEED;
    }
    else if (fanState == swON && manualFlow[0])
    {
        manualFlow[0] = false;
        manualFlow[1] = true;
        fanMode = HIGH_SPEED;
    }
    
    if (swON && !manualFlow[0] && fanMode == LOW_SPEED)
    {
        manualFlow[0] = true;
        manualFlow[1] = false;
        fanMode = LOW_SPEED;
    }
    else if (swON && manualFlow[0] && fanMode == LOW_SPEED)
    {
        manualFlow[0] = false;
        manualFlow[1] = false;
        fanMode = LOW_SPEED;
    }
    else if (swON && !manualFlow[1] && fanMode == HIGH_SPEED)
    {
        manualFlow[0] = false;
        manualFlow[1] = true;
        fanMode = HIGH_SPEED;
    }
    else if (swON && manualFlow[1] && fanMode == HIGH_SPEED)
    {
        manualFlow[0] = false;
        manualFlow[1] = false;
        fanMode = HIGH_SPEED;
    }

    switch (mode)
    {
    case AUTO:
        mode = MANUAL;
        break;

    case MANUAL:
        manualFlow[0] = false;
        manualFlow[1] = false;
        fanMode = LOW_SPEED;
        mode = AUTO;

    default:

        break;
    }
}
