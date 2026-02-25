#include "app_strings.h"

const QHash<QString, QHash<QString, QString>>& UiStrings::dict()
{
    static const QHash<QString, QHash<QString, QString>> d = {
        // Window / Menus
        {"app_title", {
            {"en","InstagramTasker GUI"},
            {"es","InstagramTasker GUI"},
            {"de","InstagramTasker GUI"},
            {"nl","InstagramTasker GUI"},
            {"fr","InstagramTasker GUI"},
            {"it","InstagramTasker GUI"},
            {"pt","InstagramTasker GUI"},
            {"pl","InstagramTasker GUI"},
            {"cs","InstagramTasker GUI"},
            {"sk","InstagramTasker GUI"},
            {"hu","InstagramTasker GUI"},
            {"ro","InstagramTasker GUI"},
            {"sv","InstagramTasker GUI"},
            {"no","InstagramTasker GUI"},
            {"da","InstagramTasker GUI"},
            {"fi","InstagramTasker GUI"},
            {"el","InstagramTasker GUI"},
            {"ru","InstagramTasker GUI"}
        }},

        {"menu_file", {
            {"en","File"}, {"es","Archivo"}, {"de","Datei"}, {"nl","Bestand"}, {"fr","Fichier"},
            {"it","File"}, {"pt","Ficheiro"}, {"pl","Plik"}, {"cs","Soubor"}, {"sk","Súbor"},
            {"hu","Fájl"}, {"ro","Fișier"}, {"sv","Arkiv"}, {"no","Fil"}, {"da","Fil"},
            {"fi","Tiedosto"}, {"el","Αρχείο"}, {"ru","Файл"}
        }},
        {"menu_settings", {
            {"en","Settings"}, {"es","Configuración"}, {"de","Einstellungen"}, {"nl","Instellingen"}, {"fr","Paramètres"},
            {"it","Impostazioni"}, {"pt","Definições"}, {"pl","Ustawienia"}, {"cs","Nastavení"}, {"sk","Nastavenia"},
            {"hu","Beállítások"}, {"ro","Setări"}, {"sv","Inställningar"}, {"no","Innstillinger"}, {"da","Indstillinger"},
            {"fi","Asetukset"}, {"el","Ρυθμίσεις"}, {"ru","Настройки"}
        }},

        // Menu actions
        {"action_load_profiles", {
            {"en","Load Profiles..."}, {"es","Cargar perfiles..."}, {"de","Profile laden..."}, {"nl","Profielen laden..."}, {"fr","Charger des profils..."},
            {"it","Carica profili..."}, {"pt","Carregar perfis..."}, {"pl","Wczytaj profile..."}, {"cs","Načíst profily..."}, {"sk","Načítať profily..."},
            {"hu","Profilok betöltése..."}, {"ro","Încarcă profiluri..."}, {"sv","Ladda profiler..."}, {"no","Last profiler..."}, {"da","Indlæs profiler..."},
            {"fi","Lataa profiilit..."}, {"el","Φόρτωση προφίλ..."}, {"ru","Загрузить профили..."}
        }},
        {"action_new_profiles", {
            {"en","New Profiles List..."}, {"es","Nueva lista de perfiles..."}, {"de","Neue Profilliste..."}, {"nl","Nieuwe profielenlijst..."}, {"fr","Nouvelle liste de profils..."},
            {"it","Nuovo elenco profili..."}, {"pt","Nova lista de perfis..."}, {"pl","Nowa lista profili..."}, {"cs","Nový seznam profilů..."}, {"sk","Nový zoznam profilov..."},
            {"hu","Új profillista..."}, {"ro","Listă nouă de profiluri..."}, {"sv","Ny profillista..."}, {"no","Ny profilliste..."}, {"da","Ny profilliste..."},
            {"fi","Uusi profiililista..."}, {"el","Νέα λίστα προφίλ..."}, {"ru","Новый список профилей..."}
        }},
        {"action_exit", {
            {"en","Exit"}, {"es","Salir"}, {"de","Beenden"}, {"nl","Afsluiten"}, {"fr","Quitter"},
            {"it","Esci"}, {"pt","Sair"}, {"pl","Wyjście"}, {"cs","Konec"}, {"sk","Ukončiť"},
            {"hu","Kilépés"}, {"ro","Ieșire"}, {"sv","Avsluta"}, {"no","Avslutt"}, {"da","Afslut"},
            {"fi","Poistu"}, {"el","Έξοδος"}, {"ru","Выход"}
        }},
        {"action_preferences", {
            {"en","Preferences..."}, {"es","Preferencias..."}, {"de","Einstellungen..."}, {"nl","Voorkeuren..."}, {"fr","Préférences..."},
            {"it","Preferenze..."}, {"pt","Preferências..."}, {"pl","Preferencje..."}, {"cs","Předvolby..."}, {"sk","Predvoľby..."},
            {"hu","Beállítások..."}, {"ro","Preferințe..."}, {"sv","Inställningar..."}, {"no","Innstillinger..."}, {"da","Indstillinger..."},
            {"fi","Asetukset..."}, {"el","Ρυθμίσεις..."}, {"ru","Настройки..."}
        }},

        // Buttons / Labels
        {"btn_remove_missing", {
            {"en","Remove Missing"}, {"es","Quitar faltantes"}, {"de","Fehlende entfernen"}, {"nl","Ontbrekende verwijderen"}, {"fr","Supprimer manquants"},
            {"it","Rimuovi mancanti"}, {"pt","Remover ausentes"}, {"pl","Usuń brakujące"}, {"cs","Odstranit chybějící"}, {"sk","Odstrániť chýbajúce"},
            {"hu","Hiányzók eltávolítása"}, {"ro","Elimină lipsă"}, {"sv","Ta bort saknade"}, {"no","Fjern manglende"}, {"da","Fjern manglende"},
            {"fi","Poista puuttuvat"}, {"el","Αφαίρεση ελλειπόντων"}, {"ru","Удалить отсутствующие"}
        }},
        {"btn_start", {
            {"en","Start"}, {"es","Iniciar"}, {"de","Start"}, {"nl","Start"}, {"fr","Démarrer"},
            {"it","Avvia"}, {"pt","Iniciar"}, {"pl","Start"}, {"cs","Spustit"}, {"sk","Spustiť"},
            {"hu","Indítás"}, {"ro","Pornește"}, {"sv","Start"}, {"no","Start"}, {"da","Start"},
            {"fi","Käynnistä"}, {"el","Έναρξη"}, {"ru","Старт"}
        }},
        {"btn_pause", {
            {"en","Pause"}, {"es","Pausar"}, {"de","Pause"}, {"nl","Pauze"}, {"fr","Pause"},
            {"it","Pausa"}, {"pt","Pausar"}, {"pl","Pauza"}, {"cs","Pozastavit"}, {"sk","Pozastaviť"},
            {"hu","Szünet"}, {"ro","Pauză"}, {"sv","Pausa"}, {"no","Pause"}, {"da","Pause"},
            {"fi","Tauko"}, {"el","Παύση"}, {"ru","Пауза"}
        }},
        {"btn_resume", {
            {"en","Resume"}, {"es","Reanudar"}, {"de","Fortsetzen"}, {"nl","Hervatten"}, {"fr","Reprendre"},
            {"it","Riprendi"}, {"pt","Retomar"}, {"pl","Wznów"}, {"cs","Pokračovat"}, {"sk","Pokračovať"},
            {"hu","Folytatás"}, {"ro","Reia"}, {"sv","Fortsätt"}, {"no","Fortsett"}, {"da","Fortsæt"},
            {"fi","Jatka"}, {"el","Συνέχεια"}, {"ru","Продолжить"}
        }},
        {"btn_cancel", {
            {"en","Cancel"}, {"es","Cancelar"}, {"de","Abbrechen"}, {"nl","Annuleren"}, {"fr","Annuler"},
            {"it","Annulla"}, {"pt","Cancelar"}, {"pl","Anuluj"}, {"cs","Zrušit"}, {"sk","Zrušiť"},
            {"hu","Mégse"}, {"ro","Anulează"}, {"sv","Avbryt"}, {"no","Avbryt"}, {"da","Annuller"},
            {"fi","Peruuta"}, {"el","Άκυρο"}, {"ru","Отмена"}
        }},
        {"btn_clear_tasks", {
            {"en","Clear Current Tasks"}, {"es","Borrar tareas actuales"}, {"de","Aktuelle Aufgaben leeren"}, {"nl","Huidige taken wissen"}, {"fr","Effacer tâches en cours"},
            {"it","Svuota attività correnti"}, {"pt","Limpar tarefas atuais"}, {"pl","Wyczyść bieżące zadania"}, {"cs","Vymazat aktuální úlohy"}, {"sk","Vymazať aktuálne úlohy"},
            {"hu","Jelenlegi feladatok törlése"}, {"ro","Șterge sarcinile curente"}, {"sv","Rensa aktuella uppgifter"}, {"no","Tøm gjeldende oppgaver"}, {"da","Ryd aktuelle opgaver"},
            {"fi","Tyhjennä nykyiset tehtävät"}, {"el","Εκκαθάριση τρεχουσών εργασιών"}, {"ru","Очистить текущие задачи"}
        }},
        {"btn_add_profiles", {
            {"en","Add Profiles"}, {"es","Añadir perfiles"}, {"de","Profile hinzufügen"}, {"nl","Profielen toevoegen"}, {"fr","Ajouter profils"},
            {"it","Aggiungi profili"}, {"pt","Adicionar perfis"}, {"pl","Dodaj profile"}, {"cs","Přidat profily"}, {"sk","Pridať profily"},
            {"hu","Profilok hozzáadása"}, {"ro","Adaugă profiluri"}, {"sv","Lägg till profiler"}, {"no","Legg til profiler"}, {"da","Tilføj profiler"},
            {"fi","Lisää profiileja"}, {"el","Προσθήκη προφίλ"}, {"ru","Добавить профили"}
        }},
        {"chk_allow_new", {
            {"en","Allow adding new"}, {"es","Permitir añadir nuevos"}, {"de","Neue hinzufügen erlauben"}, {"nl","Nieuwe toevoegen toestaan"}, {"fr","Autoriser ajout de nouveaux"},
            {"it","Consenti aggiunta di nuovi"}, {"pt","Permitir adicionar novos"}, {"pl","Pozwól dodawać nowe"}, {"cs","Povolit přidávání nových"}, {"sk","Povoliť pridávanie nových"},
            {"hu","Újak hozzáadásának engedélyezése"}, {"ro","Permite adăugarea de noi"}, {"sv","Tillåt att lägga till nya"}, {"no","Tillat å legge til nye"}, {"da","Tillad at tilføje nye"},
            {"fi","Salli uusien lisääminen"}, {"el","Να επιτρέπεται προσθήκη νέων"}, {"ru","Разрешить добавлять новые"}
        }},
        {"lbl_profiles_count", {
            {"en","Profiles: %1"}, {"es","Perfiles: %1"}, {"de","Profile: %1"}, {"nl","Profielen: %1"}, {"fr","Profils : %1"},
            {"it","Profili: %1"}, {"pt","Perfis: %1"}, {"pl","Profile: %1"}, {"cs","Profily: %1"}, {"sk","Profily: %1"},
            {"hu","Profilok: %1"}, {"ro","Profiluri: %1"}, {"sv","Profiler: %1"}, {"no","Profiler: %1"}, {"da","Profiler: %1"},
            {"fi","Profiilit: %1"}, {"el","Προφίλ: %1"}, {"ru","Профили: %1"}
        }},
        {"lbl_active_count", {
            {"en","Active: %1"}, {"es","Activos: %1"}, {"de","Aktiv: %1"}, {"nl","Actief: %1"}, {"fr","Actifs : %1"},
            {"it","Attivi: %1"}, {"pt","Ativos: %1"}, {"pl","Aktywne: %1"}, {"cs","Aktivní: %1"}, {"sk","Aktívne: %1"},
            {"hu","Aktív: %1"}, {"ro","Active: %1"}, {"sv","Aktiva: %1"}, {"no","Aktive: %1"}, {"da","Aktive: %1"},
            {"fi","Aktiiviset: %1"}, {"el","Ενεργά: %1"}, {"ru","Активные: %1"}
        }},
        {"lbl_log", {
            {"en","Log:"}, {"es","Registro:"}, {"de","Protokoll:"}, {"nl","Log:"}, {"fr","Journal :"},
            {"it","Log:"}, {"pt","Log:"}, {"pl","Log:"}, {"cs","Log:"}, {"sk","Log:"},
            {"hu","Napló:"}, {"ro","Jurnal:"}, {"sv","Logg:"}, {"no","Logg:"}, {"da","Log:"},
            {"fi","Loki:"}, {"el","Καταγραφή:"}, {"ru","Лог:"}
        }},
        {"lbl_new_profiles", {
            {"en","New profiles:"}, {"es","Perfiles nuevos:"}, {"de","Neue Profile:"}, {"nl","Nieuwe profielen:"}, {"fr","Nouveaux profils :"},
            {"it","Nuovi profili:"}, {"pt","Novos perfis:"}, {"pl","Nowe profile:"}, {"cs","Nové profily:"}, {"sk","Nové profily:"},
            {"hu","Új profilok:"}, {"ro","Profiluri noi:"}, {"sv","Nya profiler:"}, {"no","Nye profiler:"}, {"da","Nye profiler:"},
            {"fi","Uudet profiilit:"}, {"el","Νέα προφίλ:"}, {"ru","Новые профили:"}
        }},
        {"lbl_add_new_profiles", {
            {"en","Add new profiles:"}, {"es","Añadir nuevos perfiles:"}, {"de","Neue Profile hinzufügen:"}, {"nl","Nieuwe profielen toevoegen:"}, {"fr","Ajouter nouveaux profils :"},
            {"it","Aggiungi nuovi profili:"}, {"pt","Adicionar novos perfis:"}, {"pl","Dodaj nowe profile:"}, {"cs","Přidat nové profily:"}, {"sk","Pridať nové profily:"},
            {"hu","Új profilok hozzáadása:"}, {"ro","Adaugă profiluri noi:"}, {"sv","Lägg till nya profiler:"}, {"no","Legg til nye profiler:"}, {"da","Tilføj nye profiler:"},
            {"fi","Lisää uusia profiileja:"}, {"el","Προσθήκη νέων προφίλ:"}, {"ru","Добавить новые профили:"}
        }},
        {"ph_paste_profiles", {
            {"en","Paste profiles here (one per line or space-separated)"},
            {"es","Pega perfiles aquí (uno por línea o separados por espacios)"},
            {"de","Profile hier einfügen (eine pro Zeile oder durch Leerzeichen getrennt)"},
            {"nl","Plak profielen hier (één per regel of spatie-gescheiden)"},
            {"fr","Collez les profils ici (un par ligne ou séparés par des espaces)"},
            {"it","Incolla qui i profili (uno per riga o separati da spazi)"},
            {"pt","Cole os perfis aqui (um por linha ou separados por espaço)"},
            {"pl","Wklej profile tutaj (jeden na linię lub rozdzielone spacją)"},
            {"cs","Vložte profily sem (jeden na řádek nebo oddělené mezerou)"},
            {"sk","Vložte profily sem (jeden na riadok alebo oddelené medzerou)"},
            {"hu","Profilok beillesztése ide (soronként vagy szóközzel elválasztva)"},
            {"ro","Lipește profiluri aici (unul pe linie sau separate prin spațiu)"},
            {"sv","Klistra in profiler här (en per rad eller mellanslagsseparerade)"},
            {"no","Lim inn profiler her (én per linje eller mellomromsseparert)"},
            {"da","Indsæt profiler her (én pr. linje eller adskilt af mellemrum)"},
            {"fi","Liitä profiilit tähän (yksi per rivi tai välilyönnillä erotettu)"},
            {"el","Επικολλήστε προφίλ εδώ (ένα ανά γραμμή ή χωρισμένα με κενό)"},
            {"ru","Вставь профили сюда (по одному в строке или через пробел)"}
        }},

        // Context menus
        {"ctx_clear_log", {
            {"en","Clear Log"}, {"es","Limpiar registro"}, {"de","Protokoll löschen"}, {"nl","Log wissen"}, {"fr","Effacer le journal"},
            {"it","Cancella log"}, {"pt","Limpar log"}, {"pl","Wyczyść log"}, {"cs","Vymazat log"}, {"sk","Vymazať log"},
            {"hu","Napló törlése"}, {"ro","Șterge jurnal"}, {"sv","Rensa logg"}, {"no","Tøm logg"}, {"da","Ryd log"},
            {"fi","Tyhjennä loki"}, {"el","Εκκαθάριση καταγραφής"}, {"ru","Очистить лог"}
        }},
        {"ctx_clear_new", {
            {"en","Clear New Profiles"}, {"es","Limpiar perfiles nuevos"}, {"de","Neue Profile löschen"}, {"nl","Nieuwe profielen wissen"}, {"fr","Effacer nouveaux profils"},
            {"it","Cancella nuovi profili"}, {"pt","Limpar novos perfis"}, {"pl","Wyczyść nowe profile"}, {"cs","Vymazat nové profily"}, {"sk","Vymazať nové profily"},
            {"hu","Új profilok törlése"}, {"ro","Șterge profiluri noi"}, {"sv","Rensa nya profiler"}, {"no","Tøm nye profiler"}, {"da","Ryd nye profiler"},
            {"fi","Tyhjennä uudet profiilit"}, {"el","Εκκαθάριση νέων προφίλ"}, {"ru","Очистить новые профили"}
        }},

        // Tray
        {"tray_show", {
            {"en","Show"}, {"es","Mostrar"}, {"de","Anzeigen"}, {"nl","Tonen"}, {"fr","Afficher"},
            {"it","Mostra"}, {"pt","Mostrar"}, {"pl","Pokaż"}, {"cs","Zobrazit"}, {"sk","Zobraziť"},
            {"hu","Megjelenítés"}, {"ro","Afișează"}, {"sv","Visa"}, {"no","Vis"}, {"da","Vis"},
            {"fi","Näytä"}, {"el","Εμφάνιση"}, {"ru","Показать"}
        }},
        {"tray_quit", {
            {"en","Quit"}, {"es","Salir"}, {"de","Beenden"}, {"nl","Afsluiten"}, {"fr","Quitter"},
            {"it","Esci"}, {"pt","Sair"}, {"pl","Zakończ"}, {"cs","Ukončit"}, {"sk","Ukončiť"},
            {"hu","Kilépés"}, {"ro","Ieșire"}, {"sv","Avsluta"}, {"no","Avslutt"}, {"da","Afslut"},
            {"fi","Lopeta"}, {"el","Έξοδος"}, {"ru","Выход"}
        }},

        // Messages / dialogs
        {"msg_invalid_profile", {
            {"en","Invalid profile name (contains '/'): %1"},
            {"es","Nombre de perfil no válido (contiene '/'): %1"},
            {"de","Ungültiger Profilname (enthält '/'): %1"},
            {"nl","Ongeldige profielnaam (bevat '/'): %1"},
            {"fr","Nom de profil invalide (contient '/'): %1"},
            {"it","Nome profilo non valido (contiene '/'): %1"},
            {"pt","Nome de perfil inválido (contém '/'): %1"},
            {"pl","Nieprawidłowa nazwa profilu (zawiera '/'): %1"},
            {"cs","Neplatný název profilu (obsahuje '/'): %1"},
            {"sk","Neplatný názov profilu (obsahuje '/'): %1"},
            {"hu","Érvénytelen profilnév (tartalmazza '/'): %1"},
            {"ro","Nume profil invalid (conține '/'): %1"},
            {"sv","Ogiltigt profilnamn (innehåller '/'): %1"},
            {"no","Ugyldig profilnavn (inneholder '/'): %1"},
            {"da","Ugyldigt profilnavn (indeholder '/'): %1"},
            {"fi","Virheellinen profiilinimi (sisältää '/'): %1"},
            {"el","Μη έγκυρο όνομα προφίλ (περιέχει '/'): %1"},
            {"ru","Недопустимое имя профиля (содержит '/'): %1"}
        }},
        {"dlg_save_profiles_title", {
            {"en","Save profiles file"}, {"es","Guardar archivo de perfiles"}, {"de","Profildatei speichern"}, {"nl","Profielbestand opslaan"}, {"fr","Enregistrer fichier de profils"},
            {"it","Salva file profili"}, {"pt","Guardar ficheiro de perfis"}, {"pl","Zapisz plik profili"}, {"cs","Uložit soubor profilů"}, {"sk","Uložiť súbor profilov"},
            {"hu","Profilfájl mentése"}, {"ro","Salvează fișierul de profiluri"}, {"sv","Spara profilfil"}, {"no","Lagre profilfil"}, {"da","Gem profilfil"},
            {"fi","Tallenna profiilitiedosto"}, {"el","Αποθήκευση αρχείου προφίλ"}, {"ru","Сохранить файл профилей"}
        }},

        // Log messages / dialogs
        {"log_workdir_set_to", {
            {"en","Working directory set to: %1"}, {"es","Directorio de trabajo establecido en: %1"}, {"de","Arbeitsverzeichnis gesetzt auf: %1"}, {"nl","Werkmap ingesteld op: %1"}, {"fr","Dossier de travail défini sur : %1"},
            {"it","Directory di lavoro impostata su: %1"}, {"pt","Diretório de trabalho definido para: %1"}, {"pl","Katalog roboczy ustawiony na: %1"}, {"cs","Pracovní adresář nastaven na: %1"}, {"sk","Pracovný adresár nastavený na: %1"},
            {"hu","Munkakönyvtár beállítva: %1"}, {"ro","Director de lucru setat la: %1"}, {"sv","Arbetskatalog satt till: %1"}, {"no","Arbeidsmappe satt til: %1"}, {"da","Arbejdsmappe sat til: %1"},
            {"fi","Työhakemisto asetettu: %1"}, {"el","Ο κατάλογος εργασίας ορίστηκε σε: %1"}, {"ru","Рабочая папка установлена: %1"}
        }},
        {"log_error_open_file", {
            {"en","Error opening file: %1"}, {"es","Error al abrir el archivo: %1"}, {"de","Fehler beim Öffnen der Datei: %1"}, {"nl","Fout bij openen van bestand: %1"}, {"fr","Erreur lors de l’ouverture du fichier : %1"},
            {"it","Errore durante l’apertura del file: %1"}, {"pt","Erro ao abrir o ficheiro: %1"}, {"pl","Błąd otwierania pliku: %1"}, {"cs","Chyba při otevírání souboru: %1"}, {"sk","Chyba pri otváraní súboru: %1"},
            {"hu","Hiba a fájl megnyitásakor: %1"}, {"ro","Eroare la deschiderea fișierului: %1"}, {"sv","Fel vid öppning av fil: %1"}, {"no","Feil ved åpning av fil: %1"}, {"da","Fejl ved åbning af fil: %1"},
            {"fi","Virhe tiedoston avaamisessa: %1"}, {"el","Σφάλμα κατά το άνοιγμα αρχείου: %1"}, {"ru","Ошибка открытия файла: %1"}
        }},
        {"log_loaded_profiles", {
            {"en","Loaded %1 profiles from %2"}, {"es","Cargados %1 perfiles de %2"}, {"de","%1 Profile aus %2 geladen"}, {"nl","%1 profielen geladen uit %2"}, {"fr","%1 profils chargés depuis %2"},
            {"it","Caricati %1 profili da %2"}, {"pt","Carregados %1 perfis de %2"}, {"pl","Wczytano %1 profili z %2"}, {"cs","Načteno %1 profilů z %2"}, {"sk","Načítaných %1 profilov z %2"},
            {"hu","%1 profil betöltve innen: %2"}, {"ro","Încărcate %1 profiluri din %2"}, {"sv","Laddade %1 profiler från %2"}, {"no","Lastet %1 profiler fra %2"}, {"da","Indlæste %1 profiler fra %2"},
            {"fi","Ladattiin %1 profiilia tiedostosta %2"}, {"el","Φορτώθηκαν %1 προφίλ από %2"}, {"ru","Загружено %1 профилей из %2"}
        }},
        {"log_created_new_profiles", {
            {"en","Created new profiles list: %1"}, {"es","Creada nueva lista de perfiles: %1"}, {"de","Neue Profilliste erstellt: %1"}, {"nl","Nieuwe profielenlijst gemaakt: %1"}, {"fr","Nouvelle liste de profils créée : %1"},
            {"it","Creato nuovo elenco profili: %1"}, {"pt","Criada nova lista de perfis: %1"}, {"pl","Utworzono nową listę profili: %1"}, {"cs","Vytvořen nový seznam profilů: %1"}, {"sk","Vytvorený nový zoznam profilov: %1"},
            {"hu","Új profillista létrehozva: %1"}, {"ro","Creată listă nouă de profiluri: %1"}, {"sv","Skapade ny profillista: %1"}, {"no","Opprettet ny profilliste: %1"}, {"da","Oprettede ny profilliste: %1"},
            {"fi","Luotiin uusi profiililista: %1"}, {"el","Δημιουργήθηκε νέα λίστα προφίλ: %1"}, {"ru","Создан новый список профилей: %1"}
        }},
        {"log_error_save_profiles", {
            {"en","Error saving profiles file: %1"}, {"es","Error al guardar el archivo de perfiles: %1"}, {"de","Fehler beim Speichern der Profildatei: %1"}, {"nl","Fout bij opslaan van profielbestand: %1"}, {"fr","Erreur lors de l’enregistrement du fichier de profils : %1"},
            {"it","Errore nel salvataggio del file profili: %1"}, {"pt","Erro ao guardar o ficheiro de perfis: %1"}, {"pl","Błąd zapisu pliku profili: %1"}, {"cs","Chyba při ukládání souboru profilů: %1"}, {"sk","Chyba pri ukladaní súboru profilov: %1"},
            {"hu","Hiba a profillista mentésekor: %1"}, {"ro","Eroare la salvarea fișierului de profiluri: %1"}, {"sv","Fel vid sparande av profilfil: %1"}, {"no","Feil ved lagring av profilfil: %1"}, {"da","Fejl ved gemning af profilfil: %1"},
            {"fi","Virhe profiilitiedoston tallennuksessa: %1"}, {"el","Σφάλμα κατά την αποθήκευση αρχείου προφίλ: %1"}, {"ru","Ошибка сохранения файла профилей: %1"}
        }},
        {"log_workdir_empty", {
            {"en","Working directory is empty."}, {"es","El directorio de trabajo está vacío."}, {"de","Arbeitsverzeichnis ist leer."}, {"nl","Werkmap is leeg."}, {"fr","Le dossier de travail est vide."},
            {"it","La directory di lavoro è vuota."}, {"pt","O diretório de trabalho está vazio."}, {"pl","Katalog roboczy jest pusty."}, {"cs","Pracovní adresář je prázdný."}, {"sk","Pracovný adresár je prázdny."},
            {"hu","A munkakönyvtár üres."}, {"ro","Directorul de lucru este gol."}, {"sv","Arbetskatalogen är tom."}, {"no","Arbeidsmappen er tom."}, {"da","Arbejdsmappe er tom."},
            {"fi","Työhakemisto on tyhjä."}, {"el","Ο κατάλογος εργασίας είναι κενός."}, {"ru","Рабочая папка пуста."}
        }},
        {"log_remove_missing_none", {
            {"en","Remove Missing: nothing to remove (all folders exist)."}, {"es","Quitar faltantes: nada que quitar (todas las carpetas existen)."}, {"de","Fehlende entfernen: nichts zu entfernen (alle Ordner existieren)."}, {"nl","Ontbrekende verwijderen: niets te verwijderen (alle mappen bestaan)."}, {"fr","Supprimer manquants : rien à supprimer (tous les dossiers existent)."},
            {"it","Rimuovi mancanti: nulla da rimuovere (tutte le cartelle esistono)."}, {"pt","Remover ausentes: nada a remover (todas as pastas existem)."}, {"pl","Usuń brakujące: nic do usunięcia (wszystkie foldery istnieją)."}, {"cs","Odstranit chybějící: není co odstranit (všechny složky existují)."}, {"sk","Odstrániť chýbajúce: niet čo odstrániť (všetky priečinky existujú)."},
            {"hu","Hiányzók eltávolítása: nincs mit eltávolítani (minden mappa létezik)."}, {"ro","Elimină lipsă: nimic de eliminat (toate folderele există)."}, {"sv","Ta bort saknade: inget att ta bort (alla mappar finns)."}, {"no","Fjern manglende: ingenting å fjerne (alle mapper finnes)."}, {"da","Fjern manglende: intet at fjerne (alle mapper findes)."},
            {"fi","Poista puuttuvat: ei poistettavaa (kaikki kansiot ovat olemassa)."}, {"el","Αφαίρεση ελλειπόντων: τίποτα για αφαίρεση (όλοι οι φάκελοι υπάρχουν)."}, {"ru","Удалить отсутствующие: удалять нечего (все папки существуют)."}
        }},
        {"log_remove_missing_removed", {
            {"en","Remove Missing: removed %1 from source and %2 from queue."}, {"es","Quitar faltantes: eliminados %1 de la fuente y %2 de la cola."}, {"de","Fehlende entfernen: %1 aus Quelle und %2 aus Warteschlange entfernt."}, {"nl","Ontbrekende verwijderen: %1 uit bron en %2 uit wachtrij verwijderd."}, {"fr","Supprimer manquants : %1 retirés de la source et %2 de la file."},
            {"it","Rimuovi mancanti: rimossi %1 dalla sorgente e %2 dalla coda."}, {"pt","Remover ausentes: removidos %1 da origem e %2 da fila."}, {"pl","Usuń brakujące: usunięto %1 ze źródła i %2 z kolejki."}, {"cs","Odstranit chybějící: odstraněno %1 ze zdroje a %2 z fronty."}, {"sk","Odstrániť chýbajúce: odstránené %1 zo zdroja a %2 z frontu."},
            {"hu","Hiányzók eltávolítása: %1 eltávolítva a forrásból és %2 a sorból."}, {"ro","Elimină lipsă: eliminate %1 din sursă și %2 din coadă."}, {"sv","Ta bort saknade: tog bort %1 från källan och %2 från kön."}, {"no","Fjern manglende: fjernet %1 fra kilden og %2 fra køen."}, {"da","Fjern manglende: fjernede %1 fra kilden og %2 fra køen."},
            {"fi","Poista puuttuvat: poistettu %1 lähteestä ja %2 jonosta."}, {"el","Αφαίρεση ελλειπόντων: αφαιρέθηκαν %1 από την πηγή και %2 από την ουρά."}, {"ru","Удалить отсутствующие: удалено %1 из источника и %2 из очереди."}
        }},
        {"log_error_open_resume", {
            {"en","Error opening resume.txt"}, {"es","Error al abrir resume.txt"}, {"de","Fehler beim Öffnen von resume.txt"}, {"nl","Fout bij openen van resume.txt"}, {"fr","Erreur lors de l’ouverture de resume.txt"},
            {"it","Errore durante l’apertura di resume.txt"}, {"pt","Erro ao abrir resume.txt"}, {"pl","Błąd otwierania resume.txt"}, {"cs","Chyba při otevírání resume.txt"}, {"sk","Chyba pri otváraní resume.txt"},
            {"hu","Hiba a resume.txt megnyitásakor"}, {"ro","Eroare la deschiderea resume.txt"}, {"sv","Fel vid öppning av resume.txt"}, {"no","Feil ved åpning av resume.txt"}, {"da","Fejl ved åbning af resume.txt"},
            {"fi","Virhe resume.txt-tiedoston avaamisessa"}, {"el","Σφάλμα κατά το άνοιγμα του resume.txt"}, {"ru","Ошибка открытия resume.txt"}
        }},
        {"log_loaded_from_resume", {
            {"en","Loaded %1 profiles from resume.txt into queue."}, {"es","Cargados %1 perfiles de resume.txt en la cola."}, {"de","%1 Profile aus resume.txt in die Warteschlange geladen."}, {"nl","%1 profielen uit resume.txt in de wachtrij geladen."}, {"fr","%1 profils chargés depuis resume.txt dans la file."},
            {"it","Caricati %1 profili da resume.txt nella coda."}, {"pt","Carregados %1 perfis de resume.txt para a fila."}, {"pl","Wczytano %1 profili z resume.txt do kolejki."}, {"cs","Načteno %1 profilů z resume.txt do fronty."}, {"sk","Načítaných %1 profilov z resume.txt do frontu."},
            {"hu","%1 profil betöltve a resume.txt-ből a sorba."}, {"ro","Încărcate %1 profiluri din resume.txt în coadă."}, {"sv","Laddade %1 profiler från resume.txt till kön."}, {"no","Lastet %1 profiler fra resume.txt til køen."}, {"da","Indlæste %1 profiler fra resume.txt til køen."},
            {"fi","Ladattiin %1 profiilia resume.txt-tiedostosta jonoon."}, {"el","Φορτώθηκαν %1 προφίλ από το resume.txt στην ουρά."}, {"ru","Загружено %1 профилей из resume.txt в очередь."}
        }},
        {"log_added_to_queue", {
            {"en","Added %1 profiles to queue."}, {"es","Añadidos %1 perfiles a la cola."}, {"de","%1 Profile zur Warteschlange hinzugefügt."}, {"nl","%1 profielen aan wachtrij toegevoegd."}, {"fr","%1 profils ajoutés à la file."},
            {"it","Aggiunti %1 profili alla coda."}, {"pt","Adicionados %1 perfis à fila."}, {"pl","Dodano %1 profili do kolejki."}, {"cs","Přidáno %1 profilů do fronty."}, {"sk","Pridaných %1 profilov do frontu."},
            {"hu","%1 profil hozzáadva a sorhoz."}, {"ro","Adăugate %1 profiluri în coadă."}, {"sv","Lade till %1 profiler i kön."}, {"no","La til %1 profiler i køen."}, {"da","Tilføjede %1 profiler til køen."},
            {"fi","Lisättiin %1 profiilia jonoon."}, {"el","Προστέθηκαν %1 προφίλ στην ουρά."}, {"ru","Добавлено %1 профилей в очередь."}
        }},
        {"log_no_new_added_to_queue", {
            {"en","No new profiles added to queue (duplicates ignored)."}, {"es","No se añadieron perfiles nuevos a la cola (duplicados ignorados)."}, {"de","Keine neuen Profile zur Warteschlange hinzugefügt (Duplikate ignoriert)."}, {"nl","Geen nieuwe profielen aan wachtrij toegevoegd (duplicaten genegeerd)."}, {"fr","Aucun nouveau profil ajouté à la file (doublons ignorés)."},
            {"it","Nessun nuovo profilo aggiunto alla coda (duplicati ignorati)."}, {"pt","Nenhum novo perfil adicionado à fila (duplicados ignorados)."}, {"pl","Nie dodano nowych profili do kolejki (duplikaty pominięte)."}, {"cs","Do fronty nebyly přidány žádné nové profily (duplicitní ignorovány)."}, {"sk","Do frontu neboli pridané žiadne nové profily (duplikáty ignorované)."},
            {"hu","Nem került új profil a sorba (a duplikátumok figyelmen kívül hagyva)."}, {"ro","Nu s-au adăugat profiluri noi în coadă (duplicate ignorate)."}, {"sv","Inga nya profiler lades till i kön (dubbletter ignorerades)."}, {"no","Ingen nye profiler lagt til i køen (duplikater ignorert)."}, {"da","Ingen nye profiler tilføjet til køen (dubletter ignoreret)."},
            {"fi","Jonoon ei lisätty uusia profiileja (duplikaatit ohitettiin)."}, {"el","Δεν προστέθηκαν νέα προφίλ στην ουρά (τα διπλότυπα αγνοήθηκαν)."}, {"ru","Новые профили в очередь не добавлены (дубликаты игнорированы)."}
        }},
        {"log_cleared_tasks", {
            {"en","Cleared current tasks and resume.txt."}, {"es","Se borraron las tareas actuales y resume.txt."}, {"de","Aktuelle Aufgaben und resume.txt gelöscht."}, {"nl","Huidige taken en resume.txt gewist."}, {"fr","Tâches en cours et resume.txt effacées."},
            {"it","Attività correnti e resume.txt cancellati."}, {"pt","Tarefas atuais e resume.txt limpos."}, {"pl","Wyczyszczono bieżące zadania i resume.txt."}, {"cs","Vymazány aktuální úlohy a resume.txt."}, {"sk","Vymazané aktuálne úlohy a resume.txt."},
            {"hu","Jelenlegi feladatok és resume.txt törölve."}, {"ro","Șterse sarcinile curente și resume.txt."}, {"sv","Rensade aktuella uppgifter och resume.txt."}, {"no","Tømte gjeldende oppgaver og resume.txt."}, {"da","Ryddede aktuelle opgaver og resume.txt."},
            {"fi","Tyhjennettiin nykyiset tehtävät ja resume.txt."}, {"el","Εκκαθαρίστηκαν οι τρέχουσες εργασίες και το resume.txt."}, {"ru","Очищены текущие задачи и resume.txt."}
        }},
        {"log_added_to_source_and_queue", {
            {"en","Added %1 to source and %2 to queue."}, {"es","Añadidos %1 a la lista y %2 a la cola."}, {"de","%1 zur Quelle und %2 zur Warteschlange hinzugefügt."}, {"nl","%1 aan bron en %2 aan wachtrij toegevoegd."}, {"fr","%1 ajoutés à la source et %2 à la file."},
            {"it","Aggiunti %1 alla sorgente e %2 alla coda."}, {"pt","Adicionados %1 à origem e %2 à fila."}, {"pl","Dodano %1 do źródła i %2 do kolejki."}, {"cs","Přidáno %1 do zdroje a %2 do fronty."}, {"sk","Pridané %1 do zdroja a %2 do frontu."},
            {"hu","%1 hozzáadva a forráshoz és %2 a sorhoz."}, {"ro","Adăugate %1 la sursă și %2 în coadă."}, {"sv","Lade till %1 i källan och %2 i kön."}, {"no","La til %1 i kilden og %2 i køen."}, {"da","Tilføjede %1 til kilden og %2 til køen."},
            {"fi","Lisättiin %1 lähteeseen ja %2 jonoon."}, {"el","Προστέθηκαν %1 στην πηγή και %2 στην ουρά."}, {"ru","Добавлено %1 в источник и %2 в очередь."}
        }},
        {"log_added_existing_only", {
            {"en","Added %1 existing profiles to queue (new blocked: %2)."}, {"es","Añadidos %1 perfiles existentes a la cola (nuevos bloqueados: %2)."}, {"de","%1 vorhandene Profile zur Warteschlange hinzugefügt (neue blockiert: %2)."}, {"nl","%1 bestaande profielen aan wachtrij toegevoegd (nieuwe geblokkeerd: %2)."}, {"fr","%1 profils existants ajoutés à la file (nouveaux bloqués : %2)."},
            {"it","Aggiunti %1 profili esistenti alla coda (nuovi bloccati: %2)."}, {"pt","Adicionados %1 perfis existentes à fila (novos bloqueados: %2)."}, {"pl","Dodano %1 istniejących profili do kolejki (nowe zablokowane: %2)."}, {"cs","Přidáno %1 existujících profilů do fronty (nové blokováno: %2)."}, {"sk","Pridané %1 existujúcich profilov do frontu (nové blokované: %2)."},
            {"hu","%1 meglévő profil hozzáadva a sorhoz (újak blokkolva: %2)."}, {"ro","Adăugate %1 profiluri existente în coadă (noi blocate: %2)."}, {"sv","Lade till %1 befintliga profiler i kön (nya blockerade: %2)."}, {"no","La til %1 eksisterende profiler i køen (nye blokkert: %2)."}, {"da","Tilføjede %1 eksisterende profiler til køen (nye blokeret: %2)."},
            {"fi","Lisättiin %1 olemassa olevaa profiilia jonoon (uudet estetty: %2)."}, {"el","Προστέθηκαν %1 υπάρχοντα προφίλ στην ουρά (νέα μπλοκαρίστηκαν: %2)."}, {"ru","Добавлено %1 существующих профилей в очередь (новые заблокированы: %2)."}
        }},
        {"log_error_append_profiles", {
            {"en","Error appending to profiles file: %1"}, {"es","Error al añadir al archivo de perfiles: %1"}, {"de","Fehler beim Anhängen an die Profildatei: %1"}, {"nl","Fout bij toevoegen aan profielbestand: %1"}, {"fr","Erreur lors de l’ajout au fichier de profils : %1"},
            {"it","Errore nell’aggiunta al file profili: %1"}, {"pt","Erro ao anexar ao ficheiro de perfis: %1"}, {"pl","Błąd dopisywania do pliku profili: %1"}, {"cs","Chyba při přidávání do souboru profilů: %1"}, {"sk","Chyba pri pridávaní do súboru profilov: %1"},
            {"hu","Hiba a profillista fájl bővítésekor: %1"}, {"ro","Eroare la adăugarea în fișierul de profiluri: %1"}, {"sv","Fel vid tillägg i profilfil: %1"}, {"no","Feil ved tilføying til profilfil: %1"}, {"da","Fejl ved tilføjelse til profilfil: %1"},
            {"fi","Virhe profiilitiedoston lisäyksessä: %1"}, {"el","Σφάλμα κατά την προσθήκη στο αρχείο προφίλ: %1"}, {"ru","Ошибка добавления в файл профилей: %1"}
        }},
        {"log_no_new_added_duplicates", {
            {"en","No new profiles added (duplicates ignored)."}, {"es","No se añadieron perfiles nuevos (duplicados ignorados)."}, {"de","Keine neuen Profile hinzugefügt (Duplikate ignoriert)."}, {"nl","Geen nieuwe profielen toegevoegd (duplicaten genegeerd)."}, {"fr","Aucun nouveau profil ajouté (doublons ignorés)."},
            {"it","Nessun nuovo profilo aggiunto (duplicati ignorati)."}, {"pt","Nenhum novo perfil adicionado (duplicados ignorados)."}, {"pl","Nie dodano nowych profili (duplikaty pominięte)."}, {"cs","Nebyly přidány žádné nové profily (duplicitní ignorovány)."}, {"sk","Neboli pridané žiadne nové profily (duplikáty ignorované)."},
            {"hu","Nem került hozzáadásra új profil (a duplikátumok figyelmen kívül hagyva)."}, {"ro","Nu s-au adăugat profiluri noi (duplicate ignorate)."}, {"sv","Inga nya profiler lades till (dubbletter ignorerades)."}, {"no","Ingen nye profiler lagt til (duplikater ignorert)."}, {"da","Ingen nye profiler tilføjet (dubletter ignoreret)."},
            {"fi","Uusia profiileja ei lisätty (duplikaatit ohitettiin)."}, {"el","Δεν προστέθηκαν νέα προφίλ (τα διπλότυπα αγνοήθηκαν)."}, {"ru","Новые профили не добавлены (дубликаты игнорированы)."}
        }},
        {"log_no_profiles_added", {
            {"en","No profiles added: either duplicates or not present in source list."}, {"es","No se añadieron perfiles: duplicados o no están en la lista."}, {"de","Keine Profile hinzugefügt: entweder Duplikate oder nicht in der Quellliste."}, {"nl","Geen profielen toegevoegd: duplicaten of niet aanwezig in de bronlijst."}, {"fr","Aucun profil ajouté : doublons ou absents de la liste source."},
            {"it","Nessun profilo aggiunto: duplicati o non presenti nella lista sorgente."}, {"pt","Nenhum perfil adicionado: duplicados ou ausentes na lista de origem."}, {"pl","Nie dodano profili: duplikaty lub brak na liście źródłowej."}, {"cs","Nebyly přidány žádné profily: duplicity nebo nejsou ve zdrojovém seznamu."}, {"sk","Neboli pridané žiadne profily: duplikáty alebo nie sú v zozname zdrojov."},
            {"hu","Nem lett profil hozzáadva: duplikátum vagy nincs a forráslistában."}, {"ro","Nu s-au adăugat profiluri: duplicate sau nu există în lista sursă."}, {"sv","Inga profiler lades till: dubbletter eller finns inte i källistan."}, {"no","Ingen profiler lagt til: duplikater eller ikke i kildelisten."}, {"da","Ingen profiler tilføjet: dubletter eller ikke i kildelisten."},
            {"fi","Profiileja ei lisätty: duplikaatteja tai niitä ei ole lähdelistassa."}, {"el","Δεν προστέθηκαν προφίλ: διπλότυπα ή δεν υπάρχουν στη λίστα πηγής."}, {"ru","Профили не добавлены: либо дубликаты, либо их нет в списке источника."}
        }},
        {"log_starting_processing", {
            {"en","Starting processing %1 profiles from queue."}, {"es","Iniciando el procesamiento de %1 perfiles de la cola."}, {"de","Starte Verarbeitung von %1 Profilen aus der Warteschlange."}, {"nl","Start verwerking van %1 profielen uit de wachtrij."}, {"fr","Démarrage du traitement de %1 profils depuis la file."},
            {"it","Avvio elaborazione di %1 profili dalla coda."}, {"pt","A iniciar o processamento de %1 perfis da fila."}, {"pl","Rozpoczynam przetwarzanie %1 profili z kolejki."}, {"cs","Spouštím zpracování %1 profilů z fronty."}, {"sk","Spúšťam spracovanie %1 profilov z frontu."},
            {"hu","%1 profil feldolgozásának indítása a sorból."}, {"ro","Pornesc procesarea a %1 profiluri din coadă."}, {"sv","Startar bearbetning av %1 profiler från kön."}, {"no","Starter behandling av %1 profiler fra køen."}, {"da","Starter behandling af %1 profiler fra køen."},
            {"fi","Aloitetaan %1 profiilin käsittely jonosta."}, {"el","Έναρξη επεξεργασίας %1 προφίλ από την ουρά."}, {"ru","Начинаю обработку %1 профилей из очереди."}
        }},
        {"log_processing_canceled", {
            {"en","Processing canceled."}, {"es","Procesamiento cancelado."}, {"de","Verarbeitung abgebrochen."}, {"nl","Verwerking geannuleerd."}, {"fr","Traitement annulé."},
            {"it","Elaborazione annullata."}, {"pt","Processamento cancelado."}, {"pl","Przetwarzanie anulowane."}, {"cs","Zpracování zrušeno."}, {"sk","Spracovanie zrušené."},
            {"hu","Feldolgozás megszakítva."}, {"ro","Procesare anulată."}, {"sv","Bearbetning avbruten."}, {"no","Behandling avbrutt."}, {"da","Behandling annulleret."},
            {"fi","Käsittely peruttu."}, {"el","Η επεξεργασία ακυρώθηκε."}, {"ru","Обработка отменена."}
        }},
        {"log_all_processed", {
            {"en","All profiles processed."}, {"es","Todos los perfiles procesados."}, {"de","Alle Profile verarbeitet."}, {"nl","Alle profielen verwerkt."}, {"fr","Tous les profils traités."},
            {"it","Tutti i profili elaborati."}, {"pt","Todos os perfis processados."}, {"pl","Wszystkie profile przetworzone."}, {"cs","Všechny profily zpracovány."}, {"sk","Všetky profily spracované."},
            {"hu","Minden profil feldolgozva."}, {"ro","Toate profilurile procesate."}, {"sv","Alla profiler bearbetade."}, {"no","Alle profiler behandlet."}, {"da","Alle profiler behandlet."},
            {"fi","Kaikki profiilit käsitelty."}, {"el","Όλα τα προφίλ επεξεργάστηκαν."}, {"ru","Все профили обработаны."}
        }},
        {"log_processing_profile", {
            {"en","Processing profile [%1/%2]: %3"}, {"es","Procesando perfil [%1/%2]: %3"}, {"de","Verarbeite Profil [%1/%2]: %3"}, {"nl","Verwerk profiel [%1/%2]: %3"}, {"fr","Traitement du profil [%1/%2] : %3"},
            {"it","Elaborazione profilo [%1/%2]: %3"}, {"pt","A processar perfil [%1/%2]: %3"}, {"pl","Przetwarzanie profilu [%1/%2]: %3"}, {"cs","Zpracování profilu [%1/%2]: %3"}, {"sk","Spracovanie profilu [%1/%2]: %3"},
            {"hu","Profil feldolgozása [%1/%2]: %3"}, {"ro","Procesez profilul [%1/%2]: %3"}, {"sv","Bearbetar profil [%1/%2]: %3"}, {"no","Behandler profil [%1/%2]: %3"}, {"da","Behandler profil [%1/%2]: %3"},
            {"fi","Käsitellään profiilia [%1/%2]: %3"}, {"el","Επεξεργασία προφίλ [%1/%2]: %3"}, {"ru","Обработка профиля [%1/%2]: %3"}
        }},
        {"log_failed_start_process", {
            {"en","Failed to start process for %1: %2"}, {"es","No se pudo iniciar el proceso para %1: %2"}, {"de","Prozess für %1 konnte nicht gestartet werden: %2"}, {"nl","Proces voor %1 kon niet worden gestart: %2"}, {"fr","Impossible de démarrer le processus pour %1 : %2"},
            {"it","Impossibile avviare il processo per %1: %2"}, {"pt","Falha ao iniciar o processo para %1: %2"}, {"pl","Nie udało się uruchomić procesu dla %1: %2"}, {"cs","Nepodařilo se spustit proces pro %1: %2"}, {"sk","Nepodarilo sa spustiť proces pre %1: %2"},
            {"hu","Nem sikerült elindítani a folyamatot ehhez: %1: %2"}, {"ro","Nu s-a putut porni procesul pentru %1: %2"}, {"sv","Kunde inte starta processen för %1: %2"}, {"no","Kunne ikke starte prosessen for %1: %2"}, {"da","Kunne ikke starte processen for %1: %2"},
            {"fi","Prosessin käynnistys epäonnistui profiilille %1: %2"}, {"el","Αποτυχία εκκίνησης διεργασίας για %1: %2"}, {"ru","Не удалось запустить процесс для %1: %2"}
        }},
        {"log_profile_success", {
            {"en","Profile %1 downloaded successfully."}, {"es","Perfil %1 descargado con éxito."}, {"de","Profil %1 erfolgreich heruntergeladen."}, {"nl","Profiel %1 succesvol gedownload."}, {"fr","Profil %1 téléchargé avec succès."},
            {"it","Profilo %1 scaricato con successo."}, {"pt","Perfil %1 transferido com sucesso."}, {"pl","Profil %1 pobrany pomyślnie."}, {"cs","Profil %1 úspěšně stažen."}, {"sk","Profil %1 úspešne stiahnutý."},
            {"hu","%1 profil sikeresen letöltve."}, {"ro","Profilul %1 a fost descărcat cu succes."}, {"sv","Profil %1 laddades ner."}, {"no","Profil %1 ble lastet ned."}, {"da","Profil %1 blev downloadet."},
            {"fi","Profiili %1 ladattu onnistuneesti."}, {"el","Το προφίλ %1 κατέβηκε με επιτυχία."}, {"ru","Профиль %1 успешно скачан."}
        }},
        {"log_profile_failed", {
            {"en","Profile %1 download failed with code %2."}, {"es","La descarga del perfil %1 falló con el código %2."}, {"de","Download von Profil %1 fehlgeschlagen (Code %2)."}, {"nl","Download van profiel %1 mislukt met code %2."}, {"fr","Échec du téléchargement du profil %1 (code %2)."},
            {"it","Download del profilo %1 non riuscito (codice %2)."}, {"pt","Falha no download do perfil %1 com código %2."}, {"pl","Pobieranie profilu %1 nie powiodło się (kod %2)."}, {"cs","Stažení profilu %1 se nezdařilo (kód %2)."}, {"sk","Stiahnutie profilu %1 zlyhalo (kód %2)."},
            {"hu","A %1 profil letöltése meghiúsult (kód %2)."}, {"ro","Descărcarea profilului %1 a eșuat cu codul %2."}, {"sv","Nedladdning av profil %1 misslyckades (kod %2)."}, {"no","Nedlasting av profil %1 feilet (kode %2)."}, {"da","Download af profil %1 fejlede (kode %2)."},
            {"fi","Profiilin %1 lataus epäonnistui (koodi %2)."}, {"el","Η λήψη του προφίλ %1 απέτυχε με κωδικό %2."}, {"ru","Скачивание профиля %1 завершилось ошибкой (код %2)."}
        }},
        {"log_sleeping", {
            {"en","Sleeping for %1 seconds before next profile."}, {"es","Esperando %1 segundos antes del siguiente perfil."}, {"de","Warte %1 Sekunden vor dem nächsten Profil."}, {"nl","Wacht %1 seconden voor het volgende profiel."}, {"fr","Attente de %1 secondes avant le prochain profil."},
            {"it","Attendo %1 secondi prima del prossimo profilo."}, {"pt","A aguardar %1 segundos antes do próximo perfil."}, {"pl","Czekam %1 sekund przed następnym profilem."}, {"cs","Čekám %1 sekund před dalším profilem."}, {"sk","Čakám %1 sekúnd pred ďalším profilom."},
            {"hu","%1 másodperc várakozás a következő profil előtt."}, {"ro","Aștept %1 secunde înainte de următorul profil."}, {"sv","Väntar %1 sekunder innan nästa profil."}, {"no","Venter %1 sekunder før neste profil."}, {"da","Venter %1 sekunder før næste profil."},
            {"fi","Odotetaan %1 sekuntia ennen seuraavaa profiilia."}, {"el","Αναμονή %1 δευτερολέπτων πριν το επόμενο προφίλ."}, {"ru","Жду %1 секунд перед следующим профилем."}
        }},
        {"log_error_save_resume", {
            {"en","Error saving resume.txt"}, {"es","Error al guardar resume.txt"}, {"de","Fehler beim Speichern von resume.txt"}, {"nl","Fout bij opslaan van resume.txt"}, {"fr","Erreur lors de l’enregistrement de resume.txt"},
            {"it","Errore nel salvataggio di resume.txt"}, {"pt","Erro ao guardar resume.txt"}, {"pl","Błąd zapisu resume.txt"}, {"cs","Chyba při ukládání resume.txt"}, {"sk","Chyba pri ukladaní resume.txt"},
            {"hu","Hiba a resume.txt mentésekor"}, {"ro","Eroare la salvarea resume.txt"}, {"sv","Fel vid sparande av resume.txt"}, {"no","Feil ved lagring av resume.txt"}, {"da","Fejl ved gemning af resume.txt"},
            {"fi","Virhe resume.txt-tiedoston tallennuksessa"}, {"el","Σφάλμα κατά την αποθήκευση του resume.txt"}, {"ru","Ошибка сохранения resume.txt"}
        }},
        {"log_summary", {
            {"en","Summary: Success: %1, Failed: %2, Processed: %3"}, {"es","Resumen: Correctos: %1, Fallidos: %2, Procesados: %3"}, {"de","Zusammenfassung: Erfolg: %1, Fehlgeschlagen: %2, Verarbeitet: %3"}, {"nl","Samenvatting: Succes: %1, Mislukt: %2, Verwerkt: %3"}, {"fr","Résumé : Réussis : %1, Échecs : %2, Traités : %3"},
            {"it","Riepilogo: Successi: %1, Falliti: %2, Elaborati: %3"}, {"pt","Resumo: Sucesso: %1, Falha: %2, Processados: %3"}, {"pl","Podsumowanie: Sukces: %1, Błędy: %2, Przetworzone: %3"}, {"cs","Souhrn: Úspěch: %1, Neúspěch: %2, Zpracováno: %3"}, {"sk","Súhrn: Úspech: %1, Neúspech: %2, Spracované: %3"},
            {"hu","Összegzés: Sikeres: %1, Hibás: %2, Feldolgozva: %3"}, {"ro","Rezumat: Reușite: %1, Eșuate: %2, Procesate: %3"}, {"sv","Sammanfattning: Lyckade: %1, Misslyckade: %2, Bearbetade: %3"}, {"no","Oppsummering: Vellykket: %1, Feilet: %2, Behandlet: %3"}, {"da","Opsummering: Succes: %1, Fejl: %2, Behandlet: %3"},
            {"fi","Yhteenveto: Onnistui: %1, Epäonnistui: %2, Käsitelty: %3"}, {"el","Σύνοψη: Επιτυχίες: %1, Αποτυχίες: %2, Επεξεργασμένα: %3"}, {"ru","Итого: Успех: %1, Ошибки: %2, Обработано: %3"}
        }},
        {"dlg_no_profiles_title", {
            {"en","No Profiles"}, {"es","Sin perfiles"}, {"de","Keine Profile"}, {"nl","Geen profielen"}, {"fr","Aucun profil"},
            {"it","Nessun profilo"}, {"pt","Sem perfis"}, {"pl","Brak profili"}, {"cs","Žádné profily"}, {"sk","Žiadne profily"},
            {"hu","Nincsenek profilok"}, {"ro","Fără profiluri"}, {"sv","Inga profiler"}, {"no","Ingen profiler"}, {"da","Ingen profiler"},
            {"fi","Ei profiileja"}, {"el","Χωρίς προφίλ"}, {"ru","Нет профилей"}
        }},
        {"dlg_no_profiles_text", {
            {"en","Add profiles to queue first."}, {"es","Primero añade perfiles a la cola."}, {"de","Füge zuerst Profile zur Warteschlange hinzu."}, {"nl","Voeg eerst profielen toe aan de wachtrij."}, {"fr","Ajoutez d’abord des profils à la file."},
            {"it","Aggiungi prima i profili alla coda."}, {"pt","Adicione primeiro perfis à fila."}, {"pl","Najpierw dodaj profile do kolejki."}, {"cs","Nejprve přidejte profily do fronty."}, {"sk","Najprv pridajte profily do frontu."},
            {"hu","Először adj profilokat a sorhoz."}, {"ro","Adaugă mai întâi profiluri în coadă."}, {"sv","Lägg först till profiler i kön."}, {"no","Legg først til profiler i køen."}, {"da","Tilføj først profiler til køen."},
            {"fi","Lisää ensin profiilit jonoon."}, {"el","Προσθέστε πρώτα προφίλ στην ουρά."}, {"ru","Сначала добавь профили в очередь."}
        }},
        {"dlg_checkpoint_title", {
            {"en","User Action Required"}, {"es","Se requiere acción"}, {"de","Aktion erforderlich"}, {"nl","Actie vereist"}, {"fr","Action requise"},
            {"it","Azione richiesta"}, {"pt","Ação necessária"}, {"pl","Wymagana akcja"}, {"cs","Vyžadována akce"}, {"sk","Vyžadovaná akcia"},
            {"hu","Felhasználói beavatkozás szükséges"}, {"ro","Necesită acțiune"}, {"sv","Åtgärd krävs"}, {"no","Handling kreves"}, {"da","Handling kræves"},
            {"fi","Toimenpiteitä tarvitaan"}, {"el","Απαιτείται ενέργεια"}, {"ru","Нужно действие пользователя"}
        }},
        {"dlg_checkpoint_text", {
            {"en","Instagram requires user action (checkpoint). Please check the logs and resolve manually."},
            {"es","Instagram requiere acción del usuario (checkpoint). Revisa los registros y resuélvelo manualmente."},
            {"de","Instagram erfordert eine Benutzeraktion (Checkpoint). Bitte Logs prüfen und manuell lösen."},
            {"nl","Instagram vereist gebruikersactie (checkpoint). Controleer de logs en los handmatig op."},
            {"fr","Instagram requiert une action de l’utilisateur (checkpoint). Vérifiez le journal et résolvez manuellement."},
            {"it","Instagram richiede un'azione dell'utente (checkpoint). Controlla i log e risolvi manualmente."},
            {"pt","O Instagram requer ação do usuário (checkpoint). Verifique o log e resolva manualmente."},
            {"pl","Instagram wymaga działania użytkownika (checkpoint). Sprawdź log i rozwiąż ręcznie."},
            {"cs","Instagram vyžaduje akci uživatele (checkpoint). Zkontrolujte log a vyřešte ručně."},
            {"sk","Instagram vyžaduje akciu používateľa (checkpoint). Skontrolujte log a vyriešte ručne."},
            {"hu","Az Instagram felhasználói beavatkozást kér (checkpoint). Ellenőrizd a naplót és oldd meg kézzel."},
            {"ro","Instagram necesită acțiune din partea utilizatorului (checkpoint). Verifică jurnalul și rezolvă manual."},
            {"sv","Instagram kräver användaråtgärd (checkpoint). Kontrollera loggen och lös manuellt."},
            {"no","Instagram krever brukerhandling (checkpoint). Sjekk loggen og løs manuelt."},
            {"da","Instagram kræver brugerhandling (checkpoint). Tjek loggen og løs manuelt."},
            {"fi","Instagram vaatii käyttäjän toimia (checkpoint). Tarkista loki ja ratkaise käsin."},
            {"el","Το Instagram απαιτεί ενέργεια χρήστη (checkpoint). Ελέγξτε το log και λύστε το χειροκίνητα."},
            {"ru","Instagram требует действия пользователя (checkpoint). Проверь лог и реши вручную."}
        }},

        // Preferences dialog
        {"prefs_title", {
            {"en","Preferences"}, {"es","Preferencias"}, {"de","Einstellungen"}, {"nl","Voorkeuren"}, {"fr","Préférences"},
            {"it","Preferenze"}, {"pt","Preferências"}, {"pl","Ustawienia"}, {"cs","Nastavení"}, {"sk","Nastavenia"},
            {"hu","Beállítások"}, {"ro","Preferințe"}, {"sv","Inställningar"}, {"no","Innstillinger"}, {"da","Indstillinger"},
            {"fi","Asetukset"}, {"el","Ρυθμίσεις"}, {"ru","Настройки"}
        }},
        {"prefs_tab_general", {
            {"en","General"}, {"es","General"}, {"de","Allgemein"}, {"nl","Algemeen"}, {"fr","Général"},
            {"it","Generale"}, {"pt","Geral"}, {"pl","Ogólne"}, {"cs","Obecné"}, {"sk","Všeobecné"},
            {"hu","Általános"}, {"ro","General"}, {"sv","Allmänt"}, {"no","Generelt"}, {"da","Generelt"},
            {"fi","Yleiset"}, {"el","Γενικά"}, {"ru","Общие"}
        }},
        {"prefs_working_dir", {
            {"en","Working directory:"}, {"es","Directorio de trabajo:"}, {"de","Arbeitsverzeichnis:"}, {"nl","Werkmap:"}, {"fr","Dossier de travail :"},
            {"it","Directory di lavoro:"}, {"pt","Diretório de trabalho:"}, {"pl","Katalog roboczy:"}, {"cs","Pracovní adresář:"}, {"sk","Pracovný adresár:"},
            {"hu","Munkakönyvtár:"}, {"ro","Director de lucru:"}, {"sv","Arbetskatalog:"}, {"no","Arbeidsmappe:"}, {"da","Arbejdsmappe:"},
            {"fi","Työhakemisto:"}, {"el","Κατάλογος εργασίας:"}, {"ru","Рабочая папка:"}
        }},
        {"prefs_browse", {
            {"en","Browse..."}, {"es","Examinar..."}, {"de","Durchsuchen..."}, {"nl","Bladeren..."}, {"fr","Parcourir..."},
            {"it","Sfoglia..."}, {"pt","Procurar..."}, {"pl","Przeglądaj..."}, {"cs","Procházet..."}, {"sk","Prehľadávať..."},
            {"hu","Tallózás..."}, {"ro","Răsfoiește..."}, {"sv","Bläddra..."}, {"no","Bla gjennom..."}, {"da","Gennemse..."},
            {"fi","Selaa..."}, {"el","Αναζήτηση..."}, {"ru","Обзор..."}
        }},
        {"prefs_language", {
            {"en","Language:"}, {"es","Idioma:"}, {"de","Sprache:"}, {"nl","Taal:"}, {"fr","Langue :"},
            {"it","Lingua:"}, {"pt","Idioma:"}, {"pl","Język:"}, {"cs","Jazyk:"}, {"sk","Jazyk:"},
            {"hu","Nyelv:"}, {"ro","Limbă:"}, {"sv","Språk:"}, {"no","Språk:"}, {"da","Sprog:"},
            {"fi","Kieli:"}, {"el","Γλώσσα:"}, {"ru","Язык:"}
        }},
        {"prefs_ok", {
            {"en","OK"}, {"es","Aceptar"}, {"de","OK"}, {"nl","OK"}, {"fr","OK"},
            {"it","OK"}, {"pt","OK"}, {"pl","OK"}, {"cs","OK"}, {"sk","OK"},
            {"hu","OK"}, {"ro","OK"}, {"sv","OK"}, {"no","OK"}, {"da","OK"},
            {"fi","OK"}, {"el","OK"}, {"ru","ОК"}
        }},
        {"prefs_cancel", {
            {"en","Cancel"}, {"es","Cancelar"}, {"de","Abbrechen"}, {"nl","Annuleren"}, {"fr","Annuler"},
            {"it","Annulla"}, {"pt","Cancelar"}, {"pl","Anuluj"}, {"cs","Zrušit"}, {"sk","Zrušiť"},
            {"hu","Mégse"}, {"ro","Anulează"}, {"sv","Avbryt"}, {"no","Avbryt"}, {"da","Annuller"},
            {"fi","Peruuta"}, {"el","Άκυρο"}, {"ru","Отмена"}
        }},
    };

    return d;
}

QString UiStrings::t(const QString &key, const QString &langCode)
{
    const auto &d = dict();
    auto it = d.find(key);
    if (it == d.end())
        return key;

    const auto &row = it.value();

    auto itLang = row.find(langCode);
    if (itLang != row.end())
        return itLang.value();

    // fallback to English
    auto itEn = row.find(QStringLiteral("en"));
    if (itEn != row.end())
        return itEn.value();

    // fallback to any available
    if (!row.isEmpty())
        return row.begin().value();

    return key;
}
