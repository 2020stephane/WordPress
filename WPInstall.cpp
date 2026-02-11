#include <iostream>
#include <fstream>
#include <string>
#include <filesystem> 
#include <cstdlib>

namespace fs = std::filesystem;

int main() {
    std::string nomDossier;
    std::string sourceZip = "C:\\DEFI_WEB\\WordPress\\wordpress-6.9-fr_FR.zip";
    std::string sourceElem = "C:\\DEFI_WEB\\WordPress\\elementor.3.35.3.zip";
    std::string baseDir = "C:/xampp/htdocs/Defi_Web/";
    std::cout << "     --------------------------------------------------------" << std::endl;
    std::cout << "     --- Installation automatique de WordPress dans XAMPP ---" << std::endl;
    std::cout << "     --------------------------------------------------------" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Entrez le nom du repertoire de travail : ";
    
    if (!(std::getline(std::cin, nomDossier)) || nomDossier.empty()) {
        return 1;
    }
    fs::path cheminComplet = fs::path(baseDir) / nomDossier;
    std::string destElem = "C:/xampp/htdocs/Defi_Web/" + nomDossier + "wp-content/plugins";
    try {
        if (!fs::exists(cheminComplet)) {
            if (fs::create_directories(cheminComplet)) {
                std::cout << "Le repertoire '" << cheminComplet.string() << "' a bien ete cree." << std::endl;
                std::cout << "" << std::endl;
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Erreur lors de la creation du repertoire: " << e.what() << std::endl;
        return 1;
    }
    
    std::string destZip = (cheminComplet / "wordpress.zip").string();
    
    std::string cmdCopy = "copy \"" + sourceZip + "\" \"" + destZip + "\" /Y > nul";
    
    std::cout << "Copie de l'archive Wordpress en cours..." << std::endl;
    if (std::system(cmdCopy.c_str()) == 0) {
        std::cout << "Extraction des fichiers en cours..." << std::endl;
        std::string cmdUnzip = "tar -xf \"" + destZip + "\" -C \"" + cheminComplet.string() + "\"";
        
        if (std::system(cmdUnzip.c_str()) == 0) {
            std::string cmdMove = "powershell -command \"Get-ChildItem -Path '" + cheminComplet.string() + "\\wordpress\\*' | Move-Item -Destination '" + cheminComplet.string() + "\\' -Force\"";
            std::system(cmdMove.c_str());
            fs::remove(destZip);
            fs::remove_all(cheminComplet / "wordpress");
            std::cout << "Structure des dossiers optimisee." << std::endl;
            std::cout << "" << std::endl;
        }
    }
    destZip = (cheminComplet / "elementor.zip").string();
    sourceZip = "C:\\DEFI_WEB\\WordPress\\elementor.3.35.3.zip";
    cmdCopy = "copy \"" + sourceZip + "\" \"" + destZip + "\" /Y > nul";
    
    std::cout << "Copie de l'archive elementore en cours..." << std::endl;
    if (std::system(cmdCopy.c_str()) == 0) {
        std::cout << "Extraction des fichiers en cours..." << std::endl;
        std::string cmdUnzip = "tar -xf \"" + destZip + "\" -C \"" + cheminComplet.string() + "\"";
        
        if (std::system(cmdUnzip.c_str()) == 0) {
            std::string cmdMove = "powershell -command \"Get-ChildItem -Path '" + cheminComplet.string() + "\\elementor\\*' | Move-Item -Destination '" + cheminComplet.string() + "\\wp-content\\plugins\\' -Force\"";
            std::system(cmdMove.c_str());
            fs::remove(destZip);
            fs::remove_all(cheminComplet / "elementor");
            std::cout << "Structure des dossiers optimisee." << std::endl;
            std::cout << "" << std::endl;
        }
    }

    std::string cmdDB = "C:\\xampp\\mysql\\bin\\mysql.exe -u root -e \"CREATE DATABASE IF NOT EXISTS `" + nomDossier + "`;\"";
    if (std::system(cmdDB.c_str()) == 0) {
        std::cout << "Base de donnees '" << nomDossier << "' creee." << std::endl;
    }

    fs::path cheminSample = cheminComplet / "wp-config-sample.php";
    fs::path cheminConfig = cheminComplet / "wp-config.php";

    std::ifstream fSample(cheminSample);
    std::ofstream fConfig(cheminConfig);

    if (fSample && fConfig) {
        std::string ligne;
        while (std::getline(fSample, ligne)) {
            if (ligne.find("database_name_here") != std::string::npos)
                fConfig << "define( 'DB_NAME', '" << nomDossier << "' );" << std::endl;
            else if (ligne.find("username_here") != std::string::npos)
                fConfig << "define( 'DB_USER', 'root' );" << std::endl;
            else if (ligne.find("password_here") != std::string::npos)
                fConfig << "define( 'DB_PASSWORD', '' );" << std::endl;
            else if (ligne.find("$table_prefix = 'wp_';") != std::string::npos)
                fConfig << "$table_prefix = 'az_';" << std::endl;
            else
                fConfig << ligne << "\n";
        }
        fSample.close();
        fConfig.close();
        std::cout << "wp-config.php genere." << std::endl;
        std::cout << "" << std::endl;
    }

    std::string adminUser = "admin";
    std::string adminPass = "MonMotDePasseFort123!";
    std::string adminEmail = "stephanebrisse@gmail.com";
    
    std::string cmdCLI = "php C:\\WP_CLI\\wp-cli.phar core install --path=\"" + cheminComplet.string() + 
                         "\" --url=\"localhost/Defi_Web/" + nomDossier + 
                         "\" --title=\"Mon Site\" --admin_user=\"" + adminUser + 
                         "\" --admin_password=\"" + adminPass + "\" --admin_email=\"" + adminEmail + "\"";
    std::system(cmdCLI.c_str());

    std::string url = "start http://localhost/Defi_Web/" + nomDossier + "/wp-admin";
    std::system(url.c_str());

    std::cout << "\nInstallation terminee ! Appuyez sur Entree pour quitter...";
    std::cin.get();
    
    return 0;
}