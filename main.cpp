#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "cashcodeprotocol.h"

using namespace std;

int main(int /*argc*/, char */*argv*/[])
{

    CashCodeProtocol CCValidator;
    CCValidator.ConnectValidator();
    CCValidator.PowerUpValidator();
    std::this_thread::sleep_for(std::chrono::seconds(5));

    for(int i = 0; i < 5; i++)
    {
        cout << "-----------[ TRY " << i << " ]------------\n";
        CCValidator.EnableSequence();
        std::this_thread::sleep_for(std::chrono::seconds(5));
        CCValidator.StartListening();


        std::this_thread::sleep_for(std::chrono::seconds(60));
        cout << "Total Cash: " << dec << CCValidator.m_CashReceived << endl;

        CCValidator.StopListening();
        CCValidator.DisableSequence();
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    cout << "Done\n";

    return 0;
}
