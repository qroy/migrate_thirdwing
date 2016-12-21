<?php

class ActiviteitenNodeMigration extends DrupalNode6Migration {
  protected function query() {
    $query = parent::query();

    $query->innerJoin('workflow_node','w','n.nid=w.nid');
    $query->addField('w','sid','workflow_stateid');

    return $query;
  }

  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_toegang', 4)
         ->sourceMigration('ToegangTerm');
    $this->addFieldMapping('field_activiteit_datum', 'field_datum');
    $this->addFieldMapping('field_activiteit_locatie', 'field_locatie')
         ->sourceMigration('LocatieNode');
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
         ->sourceMigration(array('RepertoireNode','ProgrammaNode'));

    $this->addFieldMapping('field_afbeeldingen', 'field_afbeeldingen')
         ->sourceMigration('Images');
    $this->addFieldMapping('field_afbeeldingen:file_class')
         ->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_afbeeldingen:preserve_files')
         ->defaultValue(TRUE);
    $this->addFieldMapping('field_afbeeldingen:alt', 'field_afbeeldingen:alt');
    $this->addFieldMapping('field_afbeeldingen:title', 'field_afbeeldingen:title');

    $this->addFieldMapping('field_activiteit_workflow','workflow_stateid');

  }

  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    if ($row->field_sleepgroep == 'I') { $row->field_sleepgroep = 1;}
    elseif ($row->field_sleepgroep == 'II') { $row->field_sleepgroep = 2;}
    elseif ($row->field_sleepgroep == 'III') { $row->field_sleepgroep = 3;}
    elseif ($row->field_sleepgroep == 'IV') { $row->field_sleepgroep = 4;}
    elseif ($row->field_sleepgroep == 'V') { $row->field_sleepgroep = 5;}
    else { $row->field_sleepgroep = ''; }

    $search = array('+','?','-','v');
    $replace = array('aanwezig','onbekend','afwezig','vervanging');

    $row->field_keyboard = str_replace($search, $replace, $row->field_keyboard);
    $row->field_gitaar = str_replace($search, $replace, $row->field_gitaar);
    $row->field_basgitaar = str_replace($search, $replace, $row->field_basgitaar);
    $row->field_drums = str_replace($search, $replace, $row->field_drums);

    if ($row->workflow_stateid == 15)     { $row->workflow_stateid = 2;}
    elseif ($row->workflow_stateid == 20) { $row->workflow_stateid = 3;}
    elseif ($row->workflow_stateid == 17) { $row->workflow_stateid = 4;}
    elseif ($row->workflow_stateid == 18) { $row->workflow_stateid = 5;}
    elseif ($row->workflow_stateid == 19) { $row->workflow_stateid = 6;}
    elseif ($row->workflow_stateid == 16) { $row->workflow_stateid = 7;}
  
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

    if ($row->field_audio_seizoen == 'Kerstrepertoire') { $row->field_audio_seizoen = 'kerst';}
    elseif ($row->field_audio_seizoen == 'Regulier Repertoire') { $row->field_audio_seizoen = 'regulier';}

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

    if ($row->field_prog_type == 'Programma onderdeel') { $row->field_prog_type = 'programma';}
    elseif ($row->field_prog_type == 'Nummer') { $row->field_prog_type = 'derden';}

    return TRUE;
  }
}

class AlbumNodeMigration extends DrupalNode6Migration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_album_activiteit', 'field_ref_activiteit')
         ->sourceMigration('ActiviteitenNode');
    $this->addFieldMapping('field_album_datum', 'field_datum');
    $this->addFieldMapping('field_toegang', 4)
         ->sourceMigration('ToegangTerm');

    $this->addFieldMapping('field_afbeeldingen', 'field_afbeeldingen')
         ->sourceMigration('Images');
    $this->addFieldMapping('field_afbeeldingen:file_class')
         ->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_afbeeldingen:preserve_files')
         ->defaultValue(TRUE);

  }

}

class LocatieNodeMigration extends DrupalNode6Migration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_locatie_adres', 'field_l_adres');
    $this->addFieldMapping('field_locatie_postcode', 'field_l_postcode');
    $this->addFieldMapping('field_locatie_plaats', 'field_l_plaats');
    $this->addFieldMapping('field_locatie_route', 'field_l_route');
    $this->addFieldMapping('field_locatie_route:format', 'field_l_route:format')
          ->callbacks(array($this, 'mapFormat'));

  }

}

class PaginaNodeMigration extends DrupalNode6Migration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_toegang', 4)
         ->sourceMigration('ToegangTerm');

    $this->addFieldMapping('field_afbeeldingen', 'field_afbeeldingen')
         ->sourceMigration('Images');
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

  }

}