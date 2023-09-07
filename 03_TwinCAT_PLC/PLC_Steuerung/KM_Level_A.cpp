/********************************************************************************************************************************************/
/*                                                                                                                                          */
/*  Source-Datei: KM_Level_A.cpp                                                                                                            */
/*                                                                                                                                          */
/*  Prof. Dr. Lothar Zunker      09.06.2021                                                                                                 */
/*                                                                                                                                          */
/********************************************************************************************************************************************/

/************************************************************* System-Includes **************************************************************/
#include "TcPch.h"
#include "TcMath.h"
#pragma  hdrstop

/*********************************************************** Anwender-Includes **************************************************************/
#include "KM_Level_A.h"

using namespace Zunker;                        // aus KM_Level_A.h

/******************************************************** Konstruktoren / Destruktor ********************************************************/
C_KM_Level_A::C_KM_Level_A(C_KM_GlobalObjects* KM_GlobalObjects)
{
    this->KM_GlobalObjects = KM_GlobalObjects;

    // Feldbus-Initalisierung
    this->EL1014_Klemme0 = this->KM_GlobalObjects->EK1100_EtherCAT_Koppler->AddKlemme(&this->KM_GlobalObjects->m_Inputs->EL1014_DigitalInput_Klemme0,
        &this->KM_GlobalObjects->m_Outputs->EL1014_DigitalInput_Klemme0);
    this->Taster_0 = this->EL1014_Klemme0->InitChannel(0);
    this->Taster_1 = this->EL1014_Klemme0->InitChannel(1);
    this->Taster_2 = this->EL1014_Klemme0->InitChannel(2);
    this->Taster_3 = this->EL1014_Klemme0->InitChannel(3);

    this->EL1014_Klemme1 = this->KM_GlobalObjects->EK1100_EtherCAT_Koppler->AddKlemme(&this->KM_GlobalObjects->m_Inputs->EL1014_DigitalInput_Klemme1,
        &this->KM_GlobalObjects->m_Outputs->EL1014_DigitalInput_Klemme1);
    this->InduktivSensor_links = this->EL1014_Klemme1->InitChannel(0);
    this->InduktivSensor_rechts = this->EL1014_Klemme1->InitChannel(1);

    this->EL2004_Klemme = this->KM_GlobalObjects->EK1100_EtherCAT_Koppler->AddKlemme(&this->KM_GlobalObjects->m_Inputs->EL2004_DigitalOutput_Klemme0,
        &this->KM_GlobalObjects->m_Outputs->EL2004_DigitalOutput_Klemme0);
    this->Led_0 = this->EL2004_Klemme->InitChannel(0);
    this->Led_1 = this->EL2004_Klemme->InitChannel(1);
    this->Led_2 = this->EL2004_Klemme->InitChannel(2);
    this->Led_3 = this->EL2004_Klemme->InitChannel(3);

    this->EL3162_Klemme = this->KM_GlobalObjects->EK1100_EtherCAT_Koppler->AddKlemme(&this->KM_GlobalObjects->m_Inputs->EL3162_AnalogInput_Klemme,
        &this->KM_GlobalObjects->m_Outputs->EL3162_AnalogInput_Klemme);
    this->Slider0 = this->EL3162_Klemme->InitChannel(0);

    this->EL4132_Klemme = this->KM_GlobalObjects->EK1100_EtherCAT_Koppler->AddKlemme(&this->KM_GlobalObjects->m_Inputs->EL4132_AnalogOutput_Klemme,
        &this->KM_GlobalObjects->m_Outputs->EL4132_AnalogOutput_Klemme);
    this->Led_Anzeige0 = this->EL4132_Klemme->InitChannel(0);

    this->EL5151_Klemme0 = this->KM_GlobalObjects->EK1100_EtherCAT_Koppler->AddKlemme(&this->KM_GlobalObjects->m_Inputs->EL5151_Drehgeber_Klemme,
        &this->KM_GlobalObjects->m_Outputs->EL5151_Drehgeber_Klemme);
    this->Drehgeber = this->EL5151_Klemme0->InitChannel(0);
    this->Drehgeber->SetCounterValueScaleFactor(1);
    this->Drehgeber->SetCounterIncrements(600.0 / (2 * PI));
    this->Drehgeber->ResetCounter();

    this->EL5151_Klemme1 = this->KM_GlobalObjects->EK1100_EtherCAT_Koppler->AddKlemme(&this->KM_GlobalObjects->m_Inputs->EL5151_Lineargeber_Klemme,
        &this->KM_GlobalObjects->m_Outputs->EL5151_Lineargeber_Klemme);
    this->Lineargeber = this->EL5151_Klemme1->InitChannel(0);
    this->Lineargeber->SetCounterIncrements(1000);
    this->Lineargeber->SetCounterValueScaleFactor(1);
    this->Lineargeber->ResetCounter();

    this->EL7201_Klemme = this->KM_GlobalObjects->EK1100_EtherCAT_Koppler->AddKlemme(&this->KM_GlobalObjects->m_Inputs->EL7201_ServoMotor_Klemme,
        &this->KM_GlobalObjects->m_Outputs->EL7201_ServoMotor_Klemme);
    this->ServoMotor = this->EL7201_Klemme->InitChannel(0);
    this->ServoMotor->SetServoMotorScaleFactor(0.25);
    this->ServoMotor->SetServoMotorIncrements(268435.0 / (2 * PI));
    this->ServoMotor->SetResolverScaleFactor(0.25);
    this->ServoMotor->SetResolverIncrements(1048576.0 / (2 * PI));

    // ADS-Kommunikations-Initalisierung
    this->ModulConnection_ID = E_ModulConnection_ID::Level_A;
    this->U2K_RequestMessage_Level_A_00 = this->KM_GlobalObjects->KM_Ads->Add_RequestMessage   <S_U2K_RequestMessage_Level_A_00>((Int32)this->ModulConnection_ID, (Int32)E_Level_A_CMD::KM_Kommando_00);
    this->K2U_ResponeMessage_Level_A_00 = this->KM_GlobalObjects->KM_Ads->Add_ResponseMessage  <S_K2U_ResponeMessage_Level_A_00>((Int32)this->ModulConnection_ID, (Int32)E_Level_A_CMD::KM_Kommando_00);
    this->U2K_RequestMessage_Level_A_01 = this->KM_GlobalObjects->KM_Ads->Add_RequestMessage   <S_U2K_RequestMessage_Level_A_01>((Int32)this->ModulConnection_ID, (Int32)E_Level_A_CMD::KM_Kommando_01);
    this->K2U_ResponeMessage_Level_A_01 = this->KM_GlobalObjects->KM_Ads->Add_ResponseMessage  <S_K2U_ResponeMessage_Level_A_01>((Int32)this->ModulConnection_ID, (Int32)E_Level_A_CMD::KM_Kommando_01);
    this->U2K_RequestMessage_Level_A_02 = this->KM_GlobalObjects->KM_Ads->Add_RequestMessage   <S_U2K_RequestMessage_Level_A_02>((Int32)this->ModulConnection_ID, (Int32)E_Level_A_CMD::KM_Kommando_02);
    this->K2U_ResponeMessage_Level_A_02 = this->KM_GlobalObjects->KM_Ads->Add_ResponseMessage  <S_K2U_ResponeMessage_Level_A_02>((Int32)this->ModulConnection_ID, (Int32)E_Level_A_CMD::KM_Kommando_02);
    this->U2K_RequestMessage_Level_A_03 = this->KM_GlobalObjects->KM_Ads->Add_RequestMessage   <S_U2K_RequestMessage_Level_A_03>((Int32)this->ModulConnection_ID, (Int32)E_Level_A_CMD::KM_Kommando_03);
    this->K2U_ResponeMessage_Level_A_03 = this->KM_GlobalObjects->KM_Ads->Add_ResponseMessage  <S_K2U_ResponeMessage_Level_A_03>((Int32)this->ModulConnection_ID, (Int32)E_Level_A_CMD::KM_Kommando_03);

    // Control-Attribute der Quasi-Parallel arbeitende Thread-Methoden
    this->SM_Thread1_Schritt = 0;
    this->SM_Thread1_Zaehler = 0;
    this->SM_Thread1_finished = false;      // Starten des 1. Threads; Im letzten Schritt startet Thread1 den Thread2 usw.

    this->SM_Thread2_Schritt = 0;
    this->SM_Thread2_Zaehler = 0;
    this->SM_Thread2_finished = false;

    this->SM_Thread3_Schritt = 0;
    this->SM_Thread3_Zaehler = 0;
    this->SM_Thread3_finished = false;

    this->SM_Thread4_Schritt = 0;
    this->SM_Thread4_Zaehler = 0;
    this->SM_Thread4_finished = false;

    this->SM_Thread5_Schritt = 0;
    this->SM_Thread5_Zaehler = 0;
    this->SM_Thread5_finished = false;

    this->correctedCounter = 0;
    this->rawCounter = 0;

    this->angleController = PIDController(1, 0, 0.2, 0.001, -1, -1);

    // Kind-Objekt-Instanziierung 
    this->KM_Level_B = new C_KM_Level_B * [Level_B_Anzahl];
    for (UInt32 i = 0; i < Level_B_Anzahl; i++)
    {
        this->KM_Level_B[i] = new C_KM_Level_B(this->KM_GlobalObjects, i);
    }
}


C_KM_Level_A::~C_KM_Level_A()
{
    // Kind-Objekt-DeInstanziierung 
    for (UInt32 i = 0; i < Level_B_Anzahl; i++)
    {
        delete (this->KM_Level_B[i]);
    }
    delete[](this->KM_Level_B);

    // Control-Attribute der Quasi-Parallel arbeitende Thread-Methoden
    this->correctedCounter = 0;
    this->rawCounter = 0;
    this->SM_Thread5_finished = true;
    this->SM_Thread5_Zaehler = 0;
    this->SM_Thread5_Schritt = 0;

    this->SM_Thread4_finished = true;
    this->SM_Thread4_Zaehler = 0;
    this->SM_Thread4_Schritt = 0;

    this->SM_Thread3_finished = true;
    this->SM_Thread3_Zaehler = 0;
    this->SM_Thread3_Schritt = 0;

    this->SM_Thread2_finished = true;
    this->SM_Thread2_Zaehler = 0;
    this->SM_Thread2_Schritt = 0;

    this->SM_Thread1_finished = true;
    this->SM_Thread1_Schritt = 0;
    this->SM_Thread1_Zaehler = 0;

    // ADS-Kommunikations-DeInitalisierung
    this->KM_GlobalObjects->KM_Ads->Release_Messages <S_U2K_RequestMessage_Level_A_03, S_K2U_ResponeMessage_Level_A_03>((Int32)this->ModulConnection_ID, (Int32)E_Level_A_CMD::KM_Kommando_03);
    this->KM_GlobalObjects->KM_Ads->Release_Messages <S_U2K_RequestMessage_Level_A_02, S_K2U_ResponeMessage_Level_A_02>((Int32)this->ModulConnection_ID, (Int32)E_Level_A_CMD::KM_Kommando_02);
    this->KM_GlobalObjects->KM_Ads->Release_Messages <S_U2K_RequestMessage_Level_A_01, S_K2U_ResponeMessage_Level_A_01>((Int32)this->ModulConnection_ID, (Int32)E_Level_A_CMD::KM_Kommando_01);
    this->KM_GlobalObjects->KM_Ads->Release_Messages <S_U2K_RequestMessage_Level_A_00, S_K2U_ResponeMessage_Level_A_00>((Int32)this->ModulConnection_ID, (Int32)E_Level_A_CMD::KM_Kommando_00);
    this->ModulConnection_ID = E_ModulConnection_ID::NoConnection;

    // Feldbus-DeInitalisierung
    this->EL7201_Klemme->DeInitChannel(this->ServoMotor);
    this->KM_GlobalObjects->EK1100_EtherCAT_Koppler->RemoveKlemme(this->EL7201_Klemme);

    this->EL5151_Klemme1->DeInitChannel(this->Lineargeber);
    this->KM_GlobalObjects->EK1100_EtherCAT_Koppler->RemoveKlemme(this->EL5151_Klemme1);

    this->EL5151_Klemme0->DeInitChannel(this->Drehgeber);
    this->KM_GlobalObjects->EK1100_EtherCAT_Koppler->RemoveKlemme(this->EL5151_Klemme0);

    this->EL4132_Klemme->DeInitChannel(this->Led_Anzeige0);
    this->KM_GlobalObjects->EK1100_EtherCAT_Koppler->RemoveKlemme(this->EL4132_Klemme);

    this->EL3162_Klemme->DeInitChannel(this->Slider0);
    this->KM_GlobalObjects->EK1100_EtherCAT_Koppler->RemoveKlemme(this->EL3162_Klemme);

    this->EL2004_Klemme->DeInitChannel(this->Led_3);
    this->EL2004_Klemme->DeInitChannel(this->Led_2);
    this->EL2004_Klemme->DeInitChannel(this->Led_1);
    this->EL2004_Klemme->DeInitChannel(this->Led_0);
    this->KM_GlobalObjects->EK1100_EtherCAT_Koppler->RemoveKlemme(this->EL2004_Klemme);

    this->EL1014_Klemme1->DeInitChannel(this->InduktivSensor_rechts);
    this->EL1014_Klemme1->DeInitChannel(this->InduktivSensor_links);
    this->KM_GlobalObjects->EK1100_EtherCAT_Koppler->RemoveKlemme(this->EL1014_Klemme1);

    this->EL1014_Klemme0->DeInitChannel(this->Taster_3);
    this->EL1014_Klemme0->DeInitChannel(this->Taster_2);
    this->EL1014_Klemme0->DeInitChannel(this->Taster_1);
    this->EL1014_Klemme0->DeInitChannel(this->Taster_0);
    this->KM_GlobalObjects->EK1100_EtherCAT_Koppler->RemoveKlemme(this->EL1014_Klemme0);

    this->KM_GlobalObjects = nullptr;
}

/*********************************************** Nicht öffentliche private Anwender-Methoden ************************************************/
void        C_KM_Level_A::Command_Interpreter(Int32 KM_Command)
{
    E_Level_A_CMD MyKM_Command = static_cast <E_Level_A_CMD> (KM_Command);

    // Interpetieren der Kommandos
    switch (MyKM_Command)
    {
    case E_Level_A_CMD::KM_Kommando_00:
    {
    }
    break;
    case E_Level_A_CMD::KM_Kommando_01:
    {
    }
    break;
    case E_Level_A_CMD::KM_Kommando_02:
    {
    }
    break;
    case E_Level_A_CMD::KM_Kommando_03:
    {
    }
    break;
    default:
    {
    }
    break;
    }  // switch (KM_Kommado)
}

// Encoder logic
Boolean        C_KM_Level_A::SM_Thread_1(void)
{
    // Drehgeber Pendel

    static long lastCounterValue = 0;
    long newCounterValue = this->KM_GlobalObjects->m_Inputs->EL5151_Pendel_drehgeber.CounterValue;
    this->rawCounter = newCounterValue;

    long difference = newCounterValue - lastCounterValue;

    if (abs(difference) < 10000) {
        this->correctedCounter += difference;
    }
    else {
        if (difference > 0) {
            lastCounterValue += 65535;
        }
        else {
            lastCounterValue -= 65535;
        }
        difference = newCounterValue - lastCounterValue;
        this->correctedCounter += difference;
    }

    this->encoderRads = (this->correctedCounter / 2400.0) * PI * 2.0 + PI;
    while (encoderRads > PI) {
        encoderRads -= PI * 2;
    }
    while (encoderRads < -PI) {
        encoderRads += PI * 2;
    }
    lastCounterValue = newCounterValue;

    //static long 
    // überlauf abfangen (16 bit)

    //if(this->Taster_0)
    //this->Led_0->ResetDigitalOut();

    return (false);
}
// human interface thread
Boolean        C_KM_Level_A::SM_Thread_2(void)
{
    static double motorSpeed = 0;

    static bool btn0State = false;
    static bool btn1State = false;
    static bool btn2State = false;
    static bool btn3State = false;

    bool btn0StateNew = this->Taster_0->GetDigitalIn();
    bool btn1StateNew = this->Taster_1->GetDigitalIn();
    bool btn2StateNew = this->Taster_2->GetDigitalIn();
    bool btn3StateNew = this->Taster_3->GetDigitalIn();

    // btn 0 down
    if (btn0StateNew && !btn0State) {
        if (!this->ServoMotor->IsServoMotorEnabled()) {
            this->ServoMotor->EnableServoMotor();
            this->angleController.reset();
        }
        else {
            this->ServoMotor->DisableServoMotor();
        }
    }
    // btn 1 down
    if (btn1StateNew && !btn1State) {
        motorSpeed += 0.1;
    }
    // btn 2 down
    if (btn2StateNew && !btn2State) {
        motorSpeed -= 0.1;
    }
    // btn 3 down
    if (btn3StateNew && !btn3State) {
        motorSpeed = 0;
        this->correctedCounter = 0;
    }

    this->Lineargeber->GetCounterValue();

    btn0State = btn0StateNew;
    btn1State = btn1StateNew;
    btn2State = btn2StateNew;
    btn3State = btn3StateNew;


    this->ServoMotor->SetServoMotorStellGeschwindigkeit(motorSpeed);
    this->ServoMotor->GetResolverIncrements();

    linearCounterValue = this->KM_GlobalObjects->m_Inputs->EL5151_Lineargeber_Klemme.ENC2_PeriodValue;

    if (this->ServoMotor->IsServoMotorEnabled()) {
        this->Led_0->SetDigitalOut();
    }
    else {
        this->Led_0->ResetDigitalOut();
    }

    return (false);
}
// Logic
Boolean        C_KM_Level_A::SM_Thread_3(void)
{
    if (this->ServoMotor->IsServoMotorEnabled() && encoderRads < PI / 16.0 && encoderRads > -PI / 16.0) {
        Led_1->SetDigitalOut();

    }
    else {
        Led_1->ResetDigitalOut();
    }
    return (false);
}
Boolean        C_KM_Level_A::SM_Thread_4(void)
{
    return (false);
}
Boolean        C_KM_Level_A::SM_Thread_5(void)
{
    return (false);
}

/******************************************************* Öffentliche Anwender-Methoden ******************************************************/
void        C_KM_Level_A::Cycle_Update(void)
{
    if (!this->SM_Thread1_finished) this->SM_Thread1_finished = this->SM_Thread_1();
    if (!this->SM_Thread2_finished) this->SM_Thread2_finished = this->SM_Thread_2();
    if (!this->SM_Thread3_finished) this->SM_Thread3_finished = this->SM_Thread_3();
    if (!this->SM_Thread4_finished) this->SM_Thread4_finished = this->SM_Thread_4();
    if (!this->SM_Thread5_finished) this->SM_Thread5_finished = this->SM_Thread_5();


    // Aufruf der Kind-Objekt-CycleUpdate-Methoden im Mutter-Objekt vgl. AdsReadWriteIndication 
    this->KM_Level_B[0]->Cycle_Update();                                          // muss zwingend in der Cycle_Update-Methode des Mutter-Objekt aufgerufen werden !!!
    this->KM_Level_B[1]->Cycle_Update();                                          // muss zwingend in der Cycle_Update-Methode des Mutter-Objekt aufgerufen werden !!!
    this->KM_Level_B[2]->Cycle_Update();                                          // muss zwingend in der Cycle_Update-Methode des Mutter-Objekt aufgerufen werden !!!
}  //  Cycle_Update()
void        C_KM_Level_A::AdsReadWriteIndication(Int32 ModulConnection_ID, Int32 KM_Command)
{
    E_ModulConnection_ID MyModulConnection_ID = static_cast<E_ModulConnection_ID> (ModulConnection_ID);

    if (MyModulConnection_ID == this->ModulConnection_ID)     // wenn, KM-Kommando für das hiesige Modul bestimmt, dann Auswertung durch den hiesige Command-Interpreter 
    {
        this->Command_Interpreter(KM_Command);
    }
    else                                                        // sonst Weitergabe an die Kind-Objekte; vgl Cycle-Update
    {
        this->KM_Level_B[0]->AdsReadWriteIndication(ModulConnection_ID, KM_Command);         // muss zwingend in der AdsReadWriteIndication-Methode des Mutter-Objekt aufgerufen werden !!!
        this->KM_Level_B[1]->AdsReadWriteIndication(ModulConnection_ID, KM_Command);         // muss zwingend in der AdsReadWriteIndication-Methode des Mutter-Objekt aufgerufen werden !!!
        this->KM_Level_B[2]->AdsReadWriteIndication(ModulConnection_ID, KM_Command);         // muss zwingend in der AdsReadWriteIndication-Methode des Mutter-Objekt aufgerufen werden !!!
    }
}  // void AdsReadWriteIndication (Int32 ModulConnection_ID, Int32 KM_Command)

