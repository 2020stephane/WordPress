#include <windows.h>
#include <string>


// Identifiants pour nos contrôles
#define ID_BOUTON 1
#define ID_SAISIE 2

// La fonction "Window Procedure" : elle gère les événements (clics, fermeture...)
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    static HWND hEdit; // Le champ de saisie

    switch (msg) {
        case WM_CREATE:
            // Créer le champ de texte
            hEdit = CreateWindow(L"EDIT", L"", 
                WS_CHILD | WS_VISIBLE | WS_BORDER, 
                20, 20, 200, 25, hwnd, (HMENU)ID_SAISIE, NULL, NULL);

            // Créer le bouton
            CreateWindow(L"BUTTON", L"Valider", 
                WS_CHILD | WS_VISIBLE, 
                20, 60, 100, 30, hwnd, (HMENU)ID_BOUTON, NULL, NULL);
            break;

        case WM_COMMAND:
            // Si on clique sur le bouton
            if (LOWORD(wp) == ID_BOUTON) {
                wchar_t buffer[256];
                GetWindowText(hEdit, buffer, 256); // Récupère le texte
                std::wstring message = std::wstring(L"Bonjour ") + buffer + L" !";
                MessageBox(hwnd, message.c_str(), L"Résultat", MB_OK);
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0); // Ferme proprement le programme
            break;

        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

// Point d'entrée principal (remplace le main classique)
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    WNDCLASS wc = {0};
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"MonAppClasse";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClass(&wc)) return -1;

    CreateWindow(L"MonAppClasse", L"Demander un nom", 
                 WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
                 100, 100, 300, 200, NULL, NULL, hInst, NULL);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) { // La boucle infinie pour garder la fenêtre ouverte
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}