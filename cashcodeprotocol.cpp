#include "cashcodeprotocol.h"

#include "commands.h"

#include <stdio.h>

/*
 * Возвращает наминал принятой/распознаной и отправленной в стек купюры.
 */
int CashCodeProtocol::CashCodeTable(byte code)
{
    if(code == 0x00)
        return 50;
    else if(code == 0x01)
        return 100;
    else if(code == 0x02)
        return 200;
    else if(code == 0x03)
        return 500;
    else if(code == 0x04)
        return 1000;
    else if(code == 0x05)
        return 5000;
    return 0;
}

CashCodeProtocol::CashCodeProtocol() : m_CashReceived(0)
{
    this->ComPort = new SerialPort(200);
}

CashCodeProtocol::~CashCodeProtocol()
{
    StopListening();
    delete ComPort;
}

int CashCodeProtocol::EnableSequence()
{
    vec_bytes result;
    if( ! this->m_IsConnected) {
        std::cout << "COM Port is not open!" << std::endl;
        return 1;
    }
    return 0;
}

int CashCodeProtocol::DisableSequence()
{
    vec_bytes result;
    if( ! this->m_IsConnected) {
        std::cout << "COM Port is not open!" << std::endl;
        return 1;
    }

    result = this->SendCommand(ValidatorCommands::EnableBillTypes, DISABLE_BILL_TYPES_WITH_ESCROW);
    return 0;
}

void CashCodeProtocol::ValidatorListener()
{
    vec_bytes result;
    try
    {
        while(true)
        {
            // boost interruption point
            boost::this_thread::sleep_for(boost::chrono::milliseconds(10));

            // POLL
            result = this->SendCommand(ValidatorCommands::Poll);

            if(result.size() < 3) {
                this->SendCommand(ValidatorCommands::Nak);
                continue;
            }

            // Проверка на ошибки, если были обнаружены ошибки посылаем команду NAK
            if(this->CheckErrors(result)) {
                std::cout << "CheckErrors: return true" << std::endl;
                this->SendCommand(ValidatorCommands::Nak);
                continue;
            }

            const PollResponseType responseType = PollResponseType(result[3]);

            if(responseType == PollResponseType::BillValidatorJammed){
                std::cout << "DROP CASSETTE OUT OF POSITION" << std::endl;
                continue;
            }

            // ACK
            if(responseType == PollResponseType::Ack) {
                std::cout << "ResponseType: PollResponseType::Ack" << std::endl;
                this->SendCommand(ValidatorCommands::Poll);
                continue;
            }

            // IDLING
            if(responseType == PollResponseType::Idling) {
                std::cout << "ResponseType: PollResponseType::Idling" << std::endl;
                this->SendCommand(ValidatorCommands::Ack);
                continue;
            }

            // Если 4 байт не равен 0х14 (IDLING)
            if(responseType != PollResponseType::Idling)
            {
                if(responseType == PollResponseType::Accepting)
                {
                    // ACCEPTING 0x15
                    std::cout << "ResponseType: PollResponseType::Accepting" << std::endl;
                    this->SendCommand(ValidatorCommands::Ack);
                }
                else if(responseType == PollResponseType::Rejecting)
                {
                    // ESCROW POSITION
                    // Если 0x1C (Rejection), купюроприемник скорее всего не распознал купюру
                    std::cout << "ResponseType: PollResponseType::Rejecting" << std::endl;
                    this->SendCommand(ValidatorCommands::Ack);
                }
                else if(responseType == PollResponseType::EscrowPosition)
                {
                    std::cout << "ResponseType: PollResponseType::EscrowPosition" << std::endl;

                    // ESCROW POSITION
                    // Купюра распознана и находится в отсеке хранения
                    this->SendCommand(ValidatorCommands::Ack);

                    /****** СДЕСЬ В ДАЛЬНЕЙШЕМ БУДЕТ ПРОВЕРКА СОСТОЯНИЯ СЕТИ ******/

                    // STACK
                    result = this->SendCommand(ValidatorCommands::Stack);
                }
                else if(responseType == PollResponseType::Stacking)
                {
                    // STACKING 0x17 отправка купюры в стек
                    std::cout << "ResponseType: PollResponseType::Stacking" << std::endl;
                    this->SendCommand(ValidatorCommands::Ack);
                }
                else if(responseType == PollResponseType::BillStacked)
                {
                    std::cout << "ResponseType: PollResponseType::BillStacked" << std::endl;
                    // Bill stacked 0x81
                    // купюра попала в стек
                    this->SendCommand(ValidatorCommands::Ack);
                    m_CashReceived += this->CashCodeTable(result[4]);
                    std::cout << "\nCASH: " << std::dec << this->CashCodeTable(result[4]) << std::endl;
                    print_b("STACKED: ", result);
                }
                else if(responseType == PollResponseType::Returning)
                {
                    std::cout << "ResponseType: PollResponseType::Returning" << std::endl;
                    // RETURNING
                    // Если четвертый бит 18h, следовательно идет процесс возврата
                    this->SendCommand(ValidatorCommands::Ack);
                }
                else if(responseType == PollResponseType::BillReturned)
                {
                    std::cout << "ResponseType: PollResponseType::BillReturned" << std::endl;
                    // BILL RETURNING
                    // Если четвертый бит 82h, следовательно купюра возвращена
                    this->SendCommand(ValidatorCommands::Ack);
                }
            }
        }
    } catch(boost::thread_interrupted&) {
        std::cout << "Listener Thread interrupted.\n";
    }
}

/*
 * Функция отправки команды купюроприемнику, после чего возвращает полученных результат от
 * купюроприемника в вите вектора байтов.
 */
vec_bytes CashCodeProtocol::SendCommand(ValidatorCommands cmd, vec_bytes Data)
{
    vec_bytes response;

    if(ValidatorCommands::Ack == cmd || ValidatorCommands::Nak == cmd)
    {
        vec_bytes bytes;
        if(cmd == ValidatorCommands::Ack)
            bytes = Pack.createResponse(ValidatorCommands::Ack);
        if(cmd == ValidatorCommands::Nak)
            bytes = Pack.createResponse(ValidatorCommands::Nak);

        if(bytes.size() != 0)
            this->ComPort->write_data(bytes);
    }
    else
    {
        Pack.set_cmd((byte)cmd);
        if(Data.size() != 0)
            Pack.set_data(Data);

        this->ComPort->write_data(Pack.getBytes());
        response = this->ComPort->readLine();
    }
    return response;
}

// Запуск потока прослушивания купюроприемника
void CashCodeProtocol::StartListening()
{
    assert(m_IsConnected);
    assert(m_thread == nullptr);

    if (m_IsConnected && m_thread == nullptr) {
        m_thread = new boost::thread(boost::bind(&CashCodeProtocol::ValidatorListener, this));
    }
}

void CashCodeProtocol::StopListening()
{
    if (m_thread) {
        m_thread->interrupt();
        m_thread->join();
        delete m_thread;
        m_thread = nullptr;
    }
}

/*
 * Функция инициации SerialPort для работы с купюроприемником
 */
int CashCodeProtocol::ConnectValidator()
{
    try
    {
        if(this->ComPort->start("COM1", 9600)) {
            this->m_IsConnected = true;
            std::cout << "COM CONNECTED!" << std::endl;
        }
        return 1;
    }
    catch(std::exception e)
    {
        this->m_IsConnected = false;
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}

/*
 * Включения купюроприемника
 */
int CashCodeProtocol::PowerUpValidator()
{
    vec_bytes result;

    if( ! this->m_IsConnected){
        throw new CashCodeError(0);
    }

    // Power UP
    result = this->SendCommand(ValidatorCommands::Poll);
    print_b("POWER UP: ", result);

    // Check result for errors
    if(this->CheckErrors(result)){
        this->SendCommand(ValidatorCommands::Nak);
    }

    const PollResponseType responseType = PollResponseType(result[3]);
    // Если CashCode вернул в 4 байте 0х19 значит он уже включен
    if(responseType == PollResponseType::UnitDisabled) {
        std::cout << "Validator ready to work!" << std::endl;
        return 0;
    }

    // Если все хорошо, отправляет команду подтверждения
    this->SendCommand(ValidatorCommands::Ack);

    // RESET
    result = this->SendCommand(ValidatorCommands::Reset);
     print_b("RESET: ", result);

    // Если купюроприемник не ответил сигналом ACK
    if(responseType != PollResponseType::Ack){
        m_LastError = 0x00;
        return m_LastError;
    }


    // Опрос купюроприемника процедура инициализации
    result = this->SendCommand(ValidatorCommands::Poll);
    print_b("POLL: ", result);
    if(this->CheckErrors(result)){
        this->SendCommand(ValidatorCommands::Nak);
    }
    this->SendCommand(ValidatorCommands::Ack);


    // Получение статуса GET_STATUS
    result = this->SendCommand(ValidatorCommands::GetStatus);
    print_b("GET_STATUS: ", result);

    // Команда GET STATUS возвращает 6 байт ответа. Если все равны 0, то статус ok и можно работать дальше, иначе ошибка
    if(result[3] != 0x00 || result[4] != 0x00 || result[5] != 0x00 ||
       result[6] != 0x00 || result[7] != 0x00 || result[8] != 0x00 )
    {
        this->m_LastError = 0x70;
    }

    // Подтверждает если все хорошо
    this->SendCommand(ValidatorCommands::Ack);

    // SET_SECURITY (в тестовом примере отправояет 3 байта (0 0 0)
    result = this->SendCommand(ValidatorCommands::SetSecurity, SECURITY_CODE);
    print_b("SET_SECURITY: ", result);

    // Если не получили от купюроприемника сигнал ACK
    if(responseType != PollResponseType::Ack){
        m_LastError = 0x00;
        return this->m_LastError;
    }

    // IDENTIFICATION
    result = this->SendCommand(ValidatorCommands::Identification);
    this->SendCommand(ValidatorCommands::Ack);
    print_b("IDENTIFICATION: ", result);

    // Опрашиваем купюроприемник должны получить команду INITIALIZE
    result = this->SendCommand(ValidatorCommands::Poll);
    print_b("INITIALIZE: ", result);

    if(this->CheckErrors(result)){
        this->SendCommand(ValidatorCommands::Nak);
    }

    this->SendCommand(ValidatorCommands::Ack);

    // POLL Должны получить команду UNIT DISABLE
    result = this->SendCommand(ValidatorCommands::Poll);
    print_b("UNIT DISABLE: ", result);

    if(this->CheckErrors(result)){
        this->SendCommand(ValidatorCommands::Nak);
        return m_LastError;
    }

    this->SendCommand(ValidatorCommands::Ack);
    this->m_IsPowerUp = true;

    return this->m_LastError;
}


/*
 * Система проверки принятых данных от купюроприемника 
 * на наличие ошибок.
 */
bool CashCodeProtocol::CheckErrors(vec_bytes &result)
{
    bool is_error = false;
    
    switch(result[3]) {
    case 0x30:
        this->m_LastError = 0x30;
        is_error = true;
        break;
    case 0x40:
        m_LastError = 0x40;
        is_error = true;
        break;
    case 0x42:
        m_LastError = 0x42;
        is_error = true;
        break;
    case 0x43:
        m_LastError = 0x43;
        is_error = true;
        break;
    case 0x44:
        m_LastError = 0x44;
        is_error = true;
        break;
    case 0x45:
        m_LastError = 0x45;
        is_error = true;
        break;
    case 0x46:
        m_LastError = 0x46;
        is_error = true;
        break;
    case 0x47:
        switch(result[4]) {
            case 0x50: m_LastError = 0x50; break;
            case 0x51: m_LastError = 0x51; break;
            case 0x52: m_LastError = 0x52; break;
            case 0x53: m_LastError = 0x53; break;
            case 0x54: m_LastError = 0x54; break;
            case 0x55: m_LastError = 0x55; break;
            case 0x56: m_LastError = 0x56; break;
            case 0x5F: m_LastError = 0x5F; break;
        default:
            m_LastError = 0x47;
        }
        is_error = true;
        break;
    case 0x1C:
        switch (result[4]) {
            case 0x60: m_LastError = 0x60; break;
            case 0x61: m_LastError = 0x61; break;
            case 0x62: m_LastError = 0x62; break;
            case 0x63: m_LastError = 0x63; break;
            case 0x64: m_LastError = 0x64; break;
            case 0x65: m_LastError = 0x65; break;
            case 0x66: m_LastError = 0x66; break;
            case 0x67: m_LastError = 0x67; break;
            case 0x68: m_LastError = 0x68; break;
            case 0x69: m_LastError = 0x69; break;
            case 0x6A: m_LastError = 0x6A; break;
            case 0x6C: m_LastError = 0x6C; break;
            case 0x6D: m_LastError = 0x6D; break;
            case 0x92: m_LastError = 0x92; break;
            case 0x93: m_LastError = 0x93; break;
            case 0x94: m_LastError = 0x94; break;
            case 0x95: m_LastError = 0x95; break;
        default:
            m_LastError = 0x1C;
            break;
        }
        is_error = true;
        break;
    }

    if(is_error)
        std::cout << "ERROR: " << std::setw(2) << std::setfill('0') << std::hex << m_LastError << std::endl;
    return is_error;
}


void CashCodeProtocol::print_b(std::string msg, vec_bytes data){
    std::cout << msg;
    for(auto byte : data)
        std::cout << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << (int)byte;
    std::cout << std::endl;
}
