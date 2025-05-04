#include <iostream> // Necessário para std::cout, std::cin
#include "capture_module.h"
#include "output_module.h"
#include "buffer_manager.h"
#include "ipc_manager.h"

// RAII para garantir a execução de "stop()" ao sair da função
class ModuleGuard {
public:
    ModuleGuard(CaptureModule& capture, OutputModule& output)
        : capture_(capture), output_(output) {
        std::cout << "ModuleGuard: Inicializando captura e saída de áudio..." << std::endl;
    }

    ~ModuleGuard() {
        // Garante que os módulos sejam parados no final
        std::cout << "ModuleGuard: Parando captura e saída de áudio..." << std::endl;
        capture_.stop();
        output_.stop();
    }

private:
    CaptureModule& capture_;
    OutputModule& output_;
};

int run_application() {
    // Inicializar gerenciadores de IPC e buffers associados
    std::cout << "Iniciando configuração dos IPCs..." << std::endl;
    
    IPCManager mic_ipc, loopback_ipc, local_ipc, meets_ipc;
    BufferManager mic_buffer(mic_ipc);
    BufferManager loopback_buffer(loopback_ipc);
    BufferManager local_buffer(local_ipc);
    BufferManager meets_buffer(meets_ipc);

    std::cout << "IPCManager e BufferManager configurados com sucesso!" << std::endl;

    // Módulos principais de captura e saída
    std::cout << "Criando módulos de captura e saída..." << std::endl;
    CaptureModule capture(mic_buffer, loopback_buffer);
    OutputModule output(local_buffer, meets_buffer);

    std::cout << "Módulos de captura e saída criados com sucesso!" << std::endl;

    // Garantir que os módulos sejam parados de forma limpa ao sair da função
    ModuleGuard guard(capture, output);

    // Iniciar os módulos de áudio
    std::cout << "Iniciando captura de áudio..." << std::endl;
    capture.start();

    std::cout << "Iniciando saída de áudio..." << std::endl;
    output.start();

    // Informar o usuário que a captura e a saída estão ativas
    std::cout << "Captura e saída de áudio iniciadas. Aguardando interação..." << std::endl;

    // Esperar interação do usuário para finalização
    std::cout << "Pressione Enter para parar a aplicação..." << std::endl;
    std::cin.get();

    // Não precisamos mais chamar capture.stop() ou output.stop(), pois o guard fará isso automaticamente

    return 0;
}

int main() {
    try {
        std::cout << "Iniciando aplicação..." << std::endl;
        return run_application();  // Executar aplicação principal
    } catch (const std::exception& e) {
        std::cerr << "Erro durante a execução: " << e.what() << std::endl;
        return 1;  // Código de erro
    }
}
