#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <stdlib.h>

int main() {
    char NomDossier[256];
    char CheminComplet[512];
    char Commande[1024];
    char cheminSample[512], cheminConfig[512];
    const char* SourceZip = "C:\\DEFI_WEB\\WordPress\\wordpress-6.9-fr_FR.zip";

    printf("--- Installation automatique de WordPress ---\n");
    printf("Entrez le nom du repertoire de travail :");
    if (fgets(NomDossier, sizeof(NomDossier), stdin) != NULL) {
        NomDossier[strcspn(NomDossier, "\r\n")] = 0;
    }
    snprintf(CheminComplet, sizeof(CheminComplet), "C:/xampp/htdocs/Defi_Web/%s", NomDossier);
    if (_mkdir(CheminComplet) == 0) {
        printf("Succes : Le repertoire '%s' a ete cree.\n", CheminComplet);
        snprintf(Commande, sizeof(Commande), "copy \"%s\" \"%s\\wordpress.zip\" /Y > nul", SourceZip, CheminComplet);
        printf("Copie de l'archive en cours...\n");
        if (system(Commande) == 0) {
           printf("Succes : wordpress.zip a ete copie dans %s\n", CheminComplet);
           printf("Extraction des fichiers en cours... Patientez...\n");
           snprintf(Commande, sizeof(Commande), 
                "powershell -command \"Expand-Archive -Path '%s\\wordpress.zip' -DestinationPath '%s' -Force\"", 
                CheminComplet, CheminComplet);
           if (system(Commande) == 0) {
              printf("Succes : wordpress a ete decompresse.");
              printf(" Optimisation de la structure des dossiers...\n");
              snprintf(Commande, sizeof(Commande), 
                "powershell -command \"Get-ChildItem -Path '%s\\wordpress\\*' | Move-Item -Destination '%s\\' -Force\"", 
                CheminComplet, CheminComplet);
              if (system(Commande) == 0) {
                 printf("\nSucces total ! WordPress est pret dans : %s\n", CheminComplet);
                 snprintf(Commande, sizeof(Commande), "del \"%s\\wordpress.zip\"", CheminComplet);
                 system(Commande);
                 snprintf(Commande, sizeof(Commande), "rmdir \"%s\\wordpress\" > nul 2>&1", CheminComplet);
                 system(Commande);
              } 
            } else {
                printf("Erreur lors de l'extraction.\n");
            }    
        }
    } else {
        printf("Erreur lors de la creation du repertoire");
        
    }
    printf("creation de la base de donnee\n");
    snprintf(Commande, sizeof(Commande), 
    "C:\\xampp\\mysql\\bin\\mysql.exe -u root -e \"CREATE DATABASE IF NOT EXISTS `%s`;\"", 
    NomDossier);
    if (system(Commande) == 0) {
        printf("Base de donnees '%s' creee avec succes.\n", NomDossier);
    } else {
        printf("Erreur : Impossible de creer la base de donnees.\n");
        printf("Assurez-vous que MySQL est demarre dans le panneau XAMPP.\n");
    }

    printf("\n--- Installation terminee ! ---\n");
    printf("URL : http://localhost/Defi_Web/%s\n", NomDossier);


snprintf(cheminSample, sizeof(cheminSample), "%s/wp-config-sample.php", CheminComplet);
snprintf(cheminConfig, sizeof(cheminConfig), "%s/wp-config.php", CheminComplet);

FILE *fSample = fopen(cheminSample, "r");
FILE *fConfig = fopen(cheminConfig, "w");

if (fSample && fConfig) {
    char ligne[1024];
    while (fgets(ligne, sizeof(ligne), fSample)) {
        if (strstr(ligne, "database_name_here")) {
            fprintf(fConfig, "define( 'DB_NAME', '%s' );\n", NomDossier);
        } else if (strstr(ligne, "username_here")) {
            fprintf(fConfig, "define( 'DB_USER', 'root' );\n", NomDossier);
        } else if (strstr(ligne, "password_here")) {
            fprintf(fConfig, "define( 'DB_PASSWORD', '' );\n", NomDossier);
        } else if (strstr(ligne, "$table_prefix = 'wp_';")) {
            fprintf(fConfig, "$table_prefix = 'az_';\n", NomDossier);
        } else {
            fputs(ligne, fConfig);
        }
    }
    fclose(fSample);
    fclose(fConfig);
    printf("Fichier wp-config.php genere avec succes.\n");
} else {
    printf("Erreur : Impossible de configurer wp-config.php.\n");
}
char adminUser[] = "admin";
char adminPass[] = "MonMotDePasseFort123!";
char adminEmail[] = "stephanebrisse@gmail.com";
char siteTitle[] = "Mon Nouveau Site WordPress";

printf("Installation du coeur de WordPress via WP-CLI...\n");

snprintf(Commande, sizeof(Commande), 
    "php C:\\WP_CLI\\wp-cli.phar core install --path=\"%s\" --url=\"localhost/Defi_Web/%s\" --title=\"%s\" --admin_user=\"%s\" --admin_password=\"%s\" --admin_email=\"%s\"", 
    CheminComplet, NomDossier, siteTitle, adminUser, adminPass, adminEmail);

if (system(Commande) == 0) {
    printf("Succes : WordPress a ete installe avec l'administrateur '%s'.\n", adminUser);
} else {
    printf("Erreur : WP-CLI n'a pas pu finaliser l'installation.\n");
}
    char url[512];
    snprintf(url, sizeof(url), "start http://localhost/Defi_Web/%s", NomDossier);
    system(url);
    printf("\nAppuyez sur Entrer pour terminer...");
    getchar();
    return 0;
}