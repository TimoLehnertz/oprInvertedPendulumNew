/********************************************************************************************************************************************/
/*                                                                                                                                          */
/*  Header-Datei: KM_App_Main.h                                                                                                             */
/*                                                                                                                                          */
/*  Prof. Dr. Lothar Zunker      09.06.2021                                                                                                 */
/*                                                                                                                                          */
/********************************************************************************************************************************************/
#pragma once

/************************************************************* System-Includes **************************************************************/
#include "PLC_SteuerungInterfaces.h"
#include "AdsR0.h"
#include "TcMath.h"
#include "..\..\01_Common\CommonTypes.h"              // notwendig für Deklarationen der Typen: Single, Double, Byte, Boolean, UInt16, Int16, Int32 , Int32, UInt64, Int64 (native C++-Variante zu using namespace System) 

/*********************************************************** Anwender-Includes **************************************************************/
#include "KM_GlobalObjects.h"
#include "..\..\01_Common\CD_Level_A.h"               // CommonDeclares des Moduls Level_A
#include "KM_Level_B.h"                               // Unterklasse Level_B

class PIDController {
public:

    PIDController() {
        p = 0;
        i = 0;
        d = 0;
        dt = 0;
        minI = 0;
        maxI = 0;
        isAngleController = false;
        this->setpoint = 0;
        this->integrator = 0;
        this->lastMeasurement = 0;
        this->firstMeasurementTaken = false;
    }

    PIDController(float p, float i, float d, float dt, float minI, float maxI) : p(p), i(i), d(d), dt(dt), minI(minI), maxI(maxI) {
        this->setpoint = 0;
        this->integrator = 0;
        this->lastMeasurement = 0;
        this->firstMeasurementTaken = false;
        this->isAngleController = false;
        reset();
    }

    // returns stellwert
    float update(float measurement) {
        if (!firstMeasurementTaken) {
            this->lastMeasurement = measurement;
            firstMeasurementTaken = true;
        }
        float error = measurement - this->setpoint;

        float pTerm = error * this->p;

        this->integrator += error * this->dt * this->i;
        if (this->integrator > this->maxI) this->integrator = this->maxI;
        if (this->integrator < this->minI) this->integrator = this->minI;

        float dTerm = (measurement - this->lastMeasurement) * this->d;

        float lpf = 0.5;

        this->lastMeasurement = measurement * lpf + this->lastMeasurement * (1 - lpf);
        return pTerm + this->integrator + dTerm;
    }

    void reset() {
        this->setpoint = 0;
        this->integrator = 0;
        this->lastMeasurement = 0;
        this->firstMeasurementTaken = false;
    }

    void setSetpoint(float setpoint) {
        this->setpoint = setpoint;
    }

private:
    float p;
    float i;
    float d;
    float dt;
    float minI;
    float maxI;

    float setpoint;

    float integrator;

    float lastMeasurement;

    bool isAngleController;

    bool firstMeasurementTaken;
};

namespace Zunker
{

    class C_KM_Level_A
    {
        const Double PI = 3.141592653589793238;
        const Double Zwei_PI = 6.283185307179586476;
        const Double PI_halbe = 1.570796326794896619;

        /******************************************************** Konstruktoren / Destruktor ********************************************************/
    public:
        C_KM_Level_A(C_KM_GlobalObjects* KM_GlobalObjects);
        ~C_KM_Level_A();

        /********************************************** Nicht Öffentliche private Anwender-Attribute ************************************************/
    private:
        C_KM_GlobalObjects* KM_GlobalObjects;

        // Feldbus-Initalisierung
        C_EL1014_Klemme* EL1014_Klemme0;
        C_EL1014_Kanal* Taster_0;
        C_EL1014_Kanal* Taster_1;
        C_EL1014_Kanal* Taster_2;
        C_EL1014_Kanal* Taster_3;

        C_EL1014_Klemme* EL1014_Klemme1;
        C_EL1014_Kanal* InduktivSensor_links;
        C_EL1014_Kanal* InduktivSensor_rechts;

        C_EL2004_Klemme* EL2004_Klemme;
        C_EL2004_Kanal* Led_0;
        C_EL2004_Kanal* Led_1;
        C_EL2004_Kanal* Led_2;
        C_EL2004_Kanal* Led_3;

        C_EL3162_Klemme* EL3162_Klemme;
        C_EL3162_Kanal* Slider0;

        C_EL4132_Klemme* EL4132_Klemme;
        C_EL4132_Kanal* Led_Anzeige0;

        C_EL5151_Klemme* EL5151_Klemme0;
        C_EL5151_Kanal* Drehgeber;

        C_EL5151_Klemme* EL5151_Klemme1;
        C_EL5151_Kanal* Lineargeber;

        C_EL7201_Klemme* EL7201_Klemme;
        C_EL7201_Kanal* ServoMotor;

        // ADS-Kommunikations-Initalisierung
        E_ModulConnection_ID                            ModulConnection_ID;
        S_U2K_RequestMessage_Level_A_00* U2K_RequestMessage_Level_A_00;     // U2K_RequestMessage_SetDigitalOut aus CD_Level_A.h
        S_K2U_ResponeMessage_Level_A_00* K2U_ResponeMessage_Level_A_00;     // K2U_ResponeMessage_SetDigitalOut aus CD_Level_A.h
        S_U2K_RequestMessage_Level_A_01* U2K_RequestMessage_Level_A_01;
        S_K2U_ResponeMessage_Level_A_01* K2U_ResponeMessage_Level_A_01;
        S_U2K_RequestMessage_Level_A_02* U2K_RequestMessage_Level_A_02;
        S_K2U_ResponeMessage_Level_A_02* K2U_ResponeMessage_Level_A_02;
        S_U2K_RequestMessage_Level_A_03* U2K_RequestMessage_Level_A_03;
        S_K2U_ResponeMessage_Level_A_03* K2U_ResponeMessage_Level_A_03;

        // Quasi-Parallel arbeitende Threads
        Int32                                           SM_Thread1_Schritt;
        Int32                                           SM_Thread1_Zaehler;
        Boolean                                         SM_Thread1_finished;

        Int32                                           SM_Thread2_Schritt;
        Int32                                           SM_Thread2_Zaehler;
        Boolean                                         SM_Thread2_finished;
        Int32                                           SM_Thread2_Zaehler_Max;

        Int32                                           SM_Thread3_Schritt;
        Int32                                           SM_Thread3_Zaehler;
        Boolean                                         SM_Thread3_finished;

        Int32                                           SM_Thread4_Schritt;
        Int32                                           SM_Thread4_Zaehler;
        Boolean                                         SM_Thread4_finished;

        Int32                                           SM_Thread5_Schritt;
        Int32                                           SM_Thread5_Zaehler;
        Boolean                                         SM_Thread5_finished;

        // Drehgeber pendel
        long correctedCounter; // clock wise
        long rawCounter;
        double encoderRads;

        bool enablePID;

        long linearCounterValue;


        PIDController angleController;

        double motorPos;
        double motorSetAcc;
        double motorVel;

        /**************************************************** Öffentliche Anwender-Attribute ********************************************************/
    public:
        // Kind-Objekt-Instanziierung 
        C_KM_Level_B** KM_Level_B;

        /*********************************************** Nicht öffentliche private Anwender-Methoden ************************************************/
    private:
        void       Command_Interpreter(Int32 KM_Command);

        // Quasi-Parallel arbeitende Threads
        Boolean    SM_Thread_1(void);
        Boolean    SM_Thread_2(void);
        Boolean    SM_Thread_3(void);
        Boolean    SM_Thread_4(void);
        Boolean    SM_Thread_5(void);

        void stopPID();
        void startPID();

        /******************************************************* Öffentliche Anwender-Methoden ******************************************************/
    public:
        void       Cycle_Update(void);                                      // muss zwingend in der Cycle_Update-Methode des Mutter-Objekt aufgerufen werden !!!
        void       AdsReadWriteIndication(Int32 ModulAdress, Int32 KM_Command);       // muss zwingend in der AdsReadWriteIndication-Methode des Mutter-Objekt aufgerufen werden !!!
    }; //  class C_KM_Level_A
} // namespace Zunker