#include <Arduino.h>
#include "Configuration.h"
#include "Secret.h"
#include "Store.h"
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <WiFiUdp.h>
#include <NTP.h>

FirebaseJson content;

FirebaseAuth auth;
FirebaseConfig config;
FirebaseData fbdo;

//https://github.com/sstaub/NTP
WiFiUDP wifiUdp;
NTP ntp(wifiUdp);

String path = "chambre/evenements";
String vault = "chambre-2435295";

String journalTypes[] = {
  "OUVERTURE",
  "FERMETURE",
  "CHANGEMENT_CODE"
};

//Connection de la base de données lors du démarrage
void connectDatabase() {
  config.api_key = API_KEY;
  config.token_status_callback = tokenStatusCallback;
  auth.user.email = "";
  auth.user.password = "";

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Connexion Firebase OK");
  } else {
    Serial.printf("Erreur Firebase: %s\n",
    config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  ntp.begin();
}

//Ajout des données dans la base de données
void add(Journal type) {
  //Ajoute le type
  content.set("fields/type/stringValue", journalTypes[type]);
  //Ajoute le nom du coffre
  content.set("fields/coffre/stringValue", vault);

  //https://github.com/sstaub/NTP
  //Ajoute la date et l'heure
  ntp.update();
  content.set("fields/horodatage/timestampValue", ntp.formattedTime("%Y-%m-%dT%H:%M:%SZ"));

  //Envoye les données dans la base de données
  if (Firebase.Firestore.createDocument(&fbdo, PROJECT_ID, "", path + ntp.formattedTime("%Y-%m-%dT%H:%M:%SZ"), content.raw())) {
    Serial.println("Données envoyées");
  }
  else {
    Serial.println(fbdo.errorReason());
  }
}