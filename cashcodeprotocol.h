#ifndef CASHCODEPROTOCOL_H
#define CASHCODEPROTOCOL_H

#include <iomanip>
#include <map>
#include <string>
#include <vector>
#include <atomic>

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

#include "IBillValidatorEventsListener.h"

#include "cashcodeerrors.h"
#include "ccpackage.h"
#include "commands.h"
#include "serialport.h"


class CashCodeProtocol
{
private:
    std::string m_ComPortName;
    int m_LastError;
    bool m_Disposed;
    bool m_IsConnected;
    bool m_IsPowerUp;
    bool m_IsListening;
    bool m_IsEnableBills;
    bool m_ReturnBill;
    SerialPort *ComPort{nullptr};
    CCPackage Pack;
    boost::thread* m_thread{nullptr};

    std::atomic<int> m_CashReceived;
    std::map<uint8_t, int> m_cashCodeTable;

    IBillValidatorEventsListener* m_eventsListener{nullptr};

    // Time-out ожидания ответа от считывателя
    const int POLL_TIMEOUT = 200;

    // Тайм-аут ожидания снятия блокировки
    const int EVENT_WAIT_HANDLER_TIMEOUT = 10000;

    vec_bytes ENABLE_BILL_TYPES_WITH_ESCROW = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    vec_bytes DISABLE_BILL_TYPES_WITH_ESCROW = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    vec_bytes SECURITY_CODE = { 0x00, 0x00, 0x00 };

    // Таблица кодов валют
    int GetDenominationFromCashCodeTable(byte code);

    // Функция прослушка купюро приемника
    void ValidatorListener();

public:
    CashCodeProtocol(IBillValidatorEventsListener* eventsListener = nullptr);
    ~CashCodeProtocol();

    enum class BillRecievedStatus { Accepted, Rejected };
    enum class BillCassetteStatus { Inplace, Removed };

    void SetCashCodeTable(const std::map<uint8_t, int>& cashCodeTable);

    bool IsConnected();

    int CashReceived() const;
    // Отправка команды купюро приемнику
    vec_bytes SendCommand(ValidatorCommands cmd, vec_bytes Data = {});

    // Начало прослушки CashCode
    void StartListening();

    // Остановка прослушки CashCode
    void StopListening();

    // Инициализация и открытие COM-порта для работы с CashCode
    int ConnectValidator();

    // Включение CashCode
    int PowerUpValidator();

    // Включение режима приема купюр
    int EnableValidator();

    // Выключение режима приема купюр
    int DisableValidator();

    // Проверка ответов купюро приемника на наличие ошибок
    bool CheckErrors(vec_bytes &result) ;

    // Enable sequence
    // Включение режима для приема купюр
    int EnableSequence(void);

    // Disable sequence
    // Выключение режима для приема купюр
    int DisableSequence(void);

    bool isValidatorListening() const;

    void print_b(std::string msg, vec_bytes);

    void print_bill_table(const vec_bytes &bill_table);
};

#endif // CASHCODEPROTOCOL_H
