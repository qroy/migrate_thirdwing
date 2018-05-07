<?php

class ActiviteitenNodeMigration extends DrupalNode6Migration {
  protected function query() {
    $query = parent::query();

    $query->leftJoin('workflow_node','w','n.nid=w.nid');
    $query->addField('w','sid','workflow_stateid');

    return $query;
  }

  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_toegang', 4)
         ->sourceMigration('Term_Toegang');
    $this->addFieldMapping('field_activiteit_datum', 'field_datum');
    $this->addFieldMapping('field_activiteit_locatie', 'field_locatie')
         ->sourceMigration('Node_Locatie');
    $this->addFieldMapping('field_activiteit_locatie_bijz', 'field_l_bijzonderheden');
    $this->addFieldMapping('field_activiteit_koor_a', 'field_tijd_aanwezig');
    $this->addFieldMapping('field_activiteit_sleepgroep', 'field_sleepgroep');
    $this->addFieldMapping('field_activiteit_sleepgroep_a', 'field_sleepgroep_aanwezig');
    $this->addFieldMapping('field_activiteit_karrijder', 'field_vervoer');
    $this->addFieldMapping('field_activiteit_soort', 5);
    $this->addFieldMapping('field_activiteit_bijzonderheden', 'field_bijzonderheden');
    $this->addFieldMapping('field_activiteit_bijzonderh_u', 'field_ledeninfo');
    $this->addFieldMapping('field_activiteit_bijzonderh_u:format', 'field_ledeninfo:format')
         ->callbacks(array($this, 'mapFormat'));
    $this->addFieldMapping('field_activiteit_kledingcode', 'field_kledingcode');
    $this->addFieldMapping('field_activiteit_toetsenist', 'field_keyboard');
    $this->addFieldMapping('field_activiteit_gitarist', 'field_gitaar');
    $this->addFieldMapping('field_activiteit_bassist', 'field_basgitaar');
    $this->addFieldMapping('field_activiteit_drummer', 'field_drums');
    $this->addFieldMapping('field_activiteit_programma', 'field_programma2')
         ->sourceMigration(array('Node_Repertoire','Node_Programma'));

    $this->addFieldMapping('field_afbeeldingen', 'field_afbeeldingen')
         ->sourceMigration('Image');
    $this->addFieldMapping('field_afbeeldingen:file_class')
         ->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_afbeeldingen:preserve_files')
         ->defaultValue(TRUE);
    $this->addFieldMapping('field_afbeeldingen:alt', 'field_afbeeldingen:alt');
    $this->addFieldMapping('field_afbeeldingen:title', 'field_afbeeldingen:title');

    $this->addFieldMapping('field_afbeeldingen_achtergrond', 'field_background')
         ->sourceMigration('Image');
    $this->addFieldMapping('field_afbeeldingen_achtergrond:file_class')
         ->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_afbeeldingen_achtergrond:preserve_files')
         ->defaultValue(TRUE);
    $this->addFieldMapping('field_afbeeldingen_achtergrond:alt')
        ->defaultValue('Achtergrond');
    $this->addFieldMapping('field_afbeeldingen_achtergrond:title')
        ->defaultValue('Achtergrond');

    $this->addFieldMapping('field_bestanden', 'field_files')
         ->sourceMigration('File');
    $this->addFieldMapping('field_bestanden:file_class')
         ->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_bestanden:preserve_files')
         ->defaultValue(TRUE);
    $this->addFieldMapping('field_bestanden:description', 'field_files:description');
    $this->addFieldMapping('field_bestanden:display', 'field_files:list');


    $this->addFieldMapping('field_activiteit_workflow','workflow_stateid');

  }

  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $search = array('+','?','-','v');
    $replace = array('1','0','3','2');

    $row->field_keyboard = str_replace($search, $replace, $row->field_keyboard);
    $row->field_gitaar = str_replace($search, $replace, $row->field_gitaar);
    $row->field_basgitaar = str_replace($search, $replace, $row->field_basgitaar);
    $row->field_drums = str_replace($search, $replace, $row->field_drums);
    
    if ($row->field_sleepgroep == 'I')       { $row->field_sleepgroep = 1;}
    elseif ($row->field_sleepgroep == 'II')  { $row->field_sleepgroep = 2;}
    elseif ($row->field_sleepgroep == 'III') { $row->field_sleepgroep = 3;}
    elseif ($row->field_sleepgroep == 'IV')  { $row->field_sleepgroep = 4;}
    elseif ($row->field_sleepgroep == 'V')   { $row->field_sleepgroep = 5;}
    elseif ($row->field_sleepgroep == '*')   { $row->field_sleepgroep = 9;}

    if ($row->workflow_stateid == 15)     { $row->workflow_stateid = 2;} // Concept
    elseif ($row->workflow_stateid == 20) { $row->workflow_stateid = 3;} // Gepubliceerd
    elseif ($row->workflow_stateid == 17) { $row->workflow_stateid = 4;} // Aangeraden
    elseif ($row->workflow_stateid == 18) { $row->workflow_stateid = 5;} // Archief
    elseif ($row->workflow_stateid == 19) { $row->workflow_stateid = 6;} // Geen Archief
    elseif ($row->workflow_stateid == 16) { $row->workflow_stateid = 7;} // Prullenmand
  
    return TRUE;
  }
}

class RepertoireNodeMigration extends DrupalNode6Migration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_repertoire_nummer', 'field_audio_nummer');
    $this->addFieldMapping('field_repertoire_seizoen', 'field_audio_seizoen');
    $this->addFieldMapping('field_repertoire_genre', 'field_rep_genre');
    $this->addFieldMapping('field_repertoire_actief', 'field_klapper');

    $this->addFieldMapping('field_repertoire_uitv', 'field_rep_uitv');
    $this->addFieldMapping('field_repertoire_uitv_jaar', 'field_rep_uitv_jaar');
    $this->addFieldMapping('field_repertoire_comp', 'field_rep_componist');
    $this->addFieldMapping('field_repertoire_comp_jaar', 'field_rep_componist_jaar');
    $this->addFieldMapping('field_repertoire_arr', 'field_rep_arr');
    $this->addFieldMapping('field_repertoire_arr_jaar', 'field_rep_arr_jaar');

  }

  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    if ($row->field_audio_seizoen == 'Kerst') { $row->field_audio_seizoen = 2;}
    elseif ($row->field_audio_seizoen == 'Regulier') { $row->field_audio_seizoen = 1;}

    if ($row->field_rep_genre == 'Pop') { $row->field_rep_genre = 1;}
    elseif ($row->field_rep_genre == 'Musical / Film') { $row->field_rep_genre = 2;}
    elseif ($row->field_rep_genre == 'Geestelijk / Gospel') { $row->field_rep_genre = 3;}

    if ($row->field_klapper == 'Ja') { $row->field_klapper = 1;}
    elseif ($row->field_klapper == 'Nee') { $row->field_klapper = 0;}

    return TRUE;
  }
}

class ProgrammaNodeMigration extends DrupalNode6Migration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_repertoire_seizoen', 'field_prog_type');
    $this->addFieldMapping('field_repertoire_nummer')
         ->description(t('Programma onderdelen krijgen standaard het nummer 0'))
         ->defaultValue('0');

  }

  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    if ($row->field_prog_type == 'programma') { $row->field_prog_type = 20;}
    elseif ($row->field_prog_type == 'nummer') { $row->field_prog_type = 10;}

    return TRUE;
  }
}

class AlbumNodeMigration extends DrupalNode6Migration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_album_activiteit', 'field_ref_activiteit')
         ->sourceMigration('Node_Activiteiten');
    $this->addFieldMapping('field_album_datum', 'field_datum');
    $this->addFieldMapping('field_toegang', 4)
         ->sourceMigration('Term_Toegang');

    $this->addFieldMapping('field_afbeeldingen', 'field_afbeeldingen')
         ->sourceMigration('Image');
    $this->addFieldMapping('field_afbeeldingen:file_class')
         ->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_afbeeldingen:preserve_files')
         ->defaultValue(TRUE);
    $this->addFieldMapping('field_afbeeldingen:alt', 'field_afbeeldingen:alt');
    $this->addFieldMapping('field_afbeeldingen:title', 'field_afbeeldingen:title');
  }

}

class LocatieNodeMigration extends DrupalNode6Migration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_locatie_adres', 'field_l_adres');
    $this->addFieldMapping('field_locatie_postcode', 'field_l_postcode');
    $this->addFieldMapping('field_locatie_plaats', 'field_l_plaats');
    $this->addFieldMapping('field_locatie_route', 'field_l_routelink');
    $this->addFieldMapping('field_locatie_route:title', 'field_l_routelink:title');

  }

}

class PaginaNodeMigration extends DrupalNode6Migration {
  protected function query() {
    $query = parent::query();

    $query->leftJoin('workflow_node','w','n.nid=w.nid');
    $query->addField('w','sid','workflow_stateid');

    return $query;
  }
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_toegang', 4)
         ->sourceMigration('Term_Toegang');

    $this->addFieldMapping('field_afbeeldingen', 'field_afbeeldingen')
         ->sourceMigration('Image');
    $this->addFieldMapping('field_afbeeldingen:file_class')
         ->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_afbeeldingen:preserve_files')
         ->defaultValue(TRUE);
    $this->addFieldMapping('field_afbeeldingen:alt', 'field_afbeeldingen:alt');
    $this->addFieldMapping('field_afbeeldingen:title', 'field_afbeeldingen:title');

    $this->addFieldMapping('field_bestanden', 'field_files')
         ->sourceMigration('File');
    $this->addFieldMapping('field_bestanden:file_class')
         ->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_bestanden:preserve_files')
         ->defaultValue(TRUE);
    $this->addFieldMapping('field_bestanden:description', 'field_files:description');
    $this->addFieldMapping('field_bestanden:display', 'field_files:list');

    $this->addFieldMapping('field_pagina_workflow','workflow_stateid');

  }

  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    if ($row->workflow_stateid == 22)     { $row->workflow_stateid = 19; } // Gepubliceerd
    elseif ($row->workflow_stateid == 23) { $row->workflow_stateid = 20;} // Aangeraden
    return TRUE;
  }

}

class NieuwsNodeMigration extends DrupalNode6Migration {
  protected function query() {
    $query = parent::query();

    $query->leftJoin('workflow_node','w','n.nid=w.nid');
    $query->addField('w','sid','workflow_stateid');

    return $query;
  }
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_toegang', 4)
         ->sourceMigration('Term_Toegang');

    $this->addFieldMapping('field_afbeeldingen', 'field_afbeeldingen')
         ->sourceMigration('Image');
    $this->addFieldMapping('field_afbeeldingen:file_class')
         ->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_afbeeldingen:preserve_files')
         ->defaultValue(TRUE);
    $this->addFieldMapping('field_afbeeldingen:alt', 'field_afbeeldingen:alt');
    $this->addFieldMapping('field_afbeeldingen:title', 'field_afbeeldingen:title');

    $this->addFieldMapping('field_bestanden', 'field_files')
         ->sourceMigration('File');
    $this->addFieldMapping('field_bestanden:file_class')
         ->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_bestanden:preserve_files')
         ->defaultValue(TRUE);
    $this->addFieldMapping('field_bestanden:description', 'field_files:description');
    $this->addFieldMapping('field_bestanden:display', 'field_files:list');

    $this->addFieldMapping('field_nieuws_workflow','workflow_stateid');

  }

  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    if ($row->workflow_stateid == 2)     { $row->workflow_stateid = 13;} // Concept
    elseif ($row->workflow_stateid == 3) { $row->workflow_stateid = 14;} // Gepubliceerd
    elseif ($row->workflow_stateid == 9) { $row->workflow_stateid = 15;} // Aangeraden
    elseif ($row->workflow_stateid == 4) { $row->workflow_stateid = 16;} // Archief
    elseif ($row->workflow_stateid == 8) { $row->workflow_stateid = 17;} // Prullenmand
  
    return TRUE;
  }

}

class VriendNodeMigration extends DrupalNode6Migration {
  protected function query() {
    $query = parent::query();

    $query->leftJoin('workflow_node','w','n.nid=w.nid');
    $query->addField('w','sid','workflow_stateid');

    return $query;
  }
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_afbeeldingen', 'field_afbeeldingen')
         ->sourceMigration('Image');
    $this->addFieldMapping('field_afbeeldingen:file_class')
         ->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_afbeeldingen:preserve_files')
         ->defaultValue(TRUE);
    $this->addFieldMapping('field_afbeeldingen:alt', 'field_afbeeldingen:alt');
    $this->addFieldMapping('field_afbeeldingen:title', 'field_afbeeldingen:title');

    $this->addFieldMapping('field_adres_plaats', 'field_woonplaats');
    $this->addFieldMapping('field_adres_website', 'field_website');
    $this->addFieldMapping('field_adres_website:title', 'field_website:title');
    $this->addFieldMapping('field_adres_website:attributes', 'field_website:attributes');

    $this->addFieldMapping('field_vriend_vanaf', 'field_vriend_vanaf');
    $this->addFieldMapping('field_vriend_tot', 'field_vriend_tot');
    $this->addFieldMapping('field_vriend_lengte', 'field_vriend_lengte');
    $this->addFieldMapping('field_vriend_soort', 'field_vriend_soort');
    $this->addFieldMapping('field_vriend_benaming', 'field_vriend_benaming');

    $this->addFieldMapping('field_vriend_workflow','workflow_stateid');
  }

  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    if ($row->field_vriend_soort == 'financieel') { $row->field_vriend_soort = 1;}
    elseif ($row->field_vriend_soort == 'niet-financieel') { $row->field_vriend_soort = 2;}
    elseif ($row->field_vriend_soort == 'materieel') { $row->field_vriend_soort = 3;}

    if ($row->field_vriend_benaming == 'vriend') { $row->field_vriend_benaming = 1;}
    elseif ($row->field_vriend_benaming == 'vriendin') { $row->field_vriend_benaming = 2;}
    elseif ($row->field_vriend_benaming == 'vrienden') { $row->field_vriend_benaming = 3;}


    if ($row->workflow_stateid == 11)     { $row->workflow_stateid = 9; } // Actief
    elseif ($row->workflow_stateid == 12) { $row->workflow_stateid = 10;} // Verlopen
    elseif ($row->workflow_stateid == 13) { $row->workflow_stateid = 11;} // Inactief
    return TRUE;
  }

}

class NieuwsbriefNodeMigration extends DrupalNode6Migration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_simplenews_term','field_nieuwsbrief');
    $this->addFieldMapping('field_nieuwsbrief_jaargang','field_jaargang');
    $this->addFieldMapping('field_nieuwsbrief_uitgave','field_uitgave');
    $this->addFieldMapping('field_nieuwsbrief_inhoud','field_inhoud')
         ->sourceMigration('Node_Nieuws');
    $this->addFieldMapping('field_toegang',4)
         ->sourceMigration('Term_Toegang');

  }

}
