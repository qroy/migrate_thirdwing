<?php

class ThirdwingUserMigration extends DrupalUser6Migration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_user_voornaam', 'field_voornaam');
    $this->addFieldMapping('field_user_achternaam', 'field_achternaam');
    $this->addFieldMapping('field_user_tussenvoegsel', 'field_achternaam_voorvoegsel');
    $this->addFieldMapping('field_adres_adres', 'field_adres');
    $this->addFieldMapping('field_adres_postcode', 'field_postcode');
    $this->addFieldMapping('field_adres_plaats', 'field_woonplaats');
    $this->addFieldMapping('field_adres_telefoon', 'field_telefoon');
    $this->addFieldMapping('field_adres_telefoon_mobiel', 'field_mobiel');
    $this->addFieldMapping('field_user_geslacht', 'field_geslacht');
    $this->addFieldMapping('field_user_lidsinds', 'field_lidsinds');
    $this->addFieldMapping('field_user_lidtot', 'field_uitkoor');
    $this->addFieldMapping('field_user_geboortedatum', 'field_geboortedatum');
    $this->addFieldMapping('field_activiteit_sleepgroep', 'field_sleepgroep_1');
    $this->addFieldMapping('field_user_koorfunctie', 'field_koor');
    $this->addFieldMapping('field_user_karrijder', 'field_karrijder');
    $this->addFieldMapping('field_user_notities', 'field_notes');
    $this->addFieldMapping('field_user_functie_bestuur', 'field_functie_bestuur');
    $this->addFieldMapping('field_user_functie_mc', 'field_functie_mc');
    $this->addFieldMapping('field_user_functie_cc', 'field_functie_concert');
    $this->addFieldMapping('field_user_functie_pr', 'field_functie_pr');
    $this->addFieldMapping('field_user_functie_fc', 'field_functie_feest');
    $this->addFieldMapping('field_user_functie_kc', 'field_functie_regie');
    $this->addFieldMapping('field_user_functie_ir', 'field_functie_ir');
  }

  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    $row->field_postcode = str_replace(' ', '', $row->field_postcode);

    if ($row->field_karrijder == '*') { $row->field_karrijder = 1;}
    
    return TRUE;
  }
}