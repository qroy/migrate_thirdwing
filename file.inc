<?php

class FileDocMigration extends DrupalFile6Migration {
  protected function query() {
    $query = parent::query();
  
    $query->condition('f.filemime',array('image/jpeg','image/png','image/gif','audio/mpeg'),'NOT IN');
    $query->condition('f.filepath','%verslagen%','NOT LIKE');
    $query->condition('f.filepath','%repertoire%','NOT LIKE');
    return $query;

  }
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('preserve_files', NULL, FALSE)
      ->defaultValue(FALSE);
    $this->addFieldMapping('file_replace', NULL, FALSE)
      ->defaultValue(FILE_EXISTS_REPLACE);

  }
}

class FileVerslagMigration extends DrupalFile6Migration {
  protected function query() {
  
    $query = Database::getConnection('default', $this->sourceConnection)
             ->select('node', 'n')
             ->fields('n')
             ->orderBy($this->newOnly ? 'f.fid' : 'f.timestamp')
             ->condition('n.type','verslag');
    $query->innerJoin('content_field_files','a','a.nid=n.nid AND a.vid=n.vid');
    $query->leftJoin('files','f','f.fid=a.field_files_fid');
    $query->leftJoin('content_field_datum','d','d.nid=n.nid AND d.vid=n.vid');
    $query->leftJoin('term_node','soort','soort.nid=n.nid AND soort.vid=n.vid');
    $query->leftJoin('term_data','soortterm','soortterm.tid=soort.tid');
    $query->fields('f');
    $query->condition('soortterm.vid',9);
    $query->addField('d','field_datum_value','field_verslag_datum');
    $query->addField('soort','tid','field_verslag_soort');
    return $query;

  }
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('preserve_files', NULL, FALSE)
      ->defaultValue(FALSE);
    $this->addFieldMapping('file_replace', NULL, FALSE)
      ->defaultValue(FILE_EXISTS_REPLACE);

    $this->addFieldMapping('field_verslag_datum', 'field_verslag_datum');
    $this->addFieldMapping('field_verslag_soort', 'field_verslag_soort');
    $this->addFieldMapping('field_toegang', 'field_toegang')
         ->sourceMigration('Term_Toegang');
  
  }
  
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    $term = array();
    $term_query = Database::getConnection('default', $this->sourceConnection)
      ->select('term_node', 't')
      ->fields('t')
      ->condition('t.nid',$row->nid)
      ->condition('t.vid',$row->vid);
    $term_query->leftJoin('term_data','td','td.tid=t.tid');
    $term_query->condition('td.vid',4);
    $term_result = $term_query->execute();
    foreach ($term_result as $term_record) {
      $term[] = $term_record->tid;
//      $message = 'NID ' . $row->nid . ' TID ' . $term_record->tid;
//      $this->queueMessage($message);
    }
    $row->field_toegang = $term;
    
    if ($row->field_verslag_soort == 131)       { $row->field_verslag_soort = 10;}
    elseif ($row->field_verslag_soort == 129)   { $row->field_verslag_soort = 20;}
    elseif ($row->field_verslag_soort == 201)   { $row->field_verslag_soort = 70;}
    elseif ($row->field_verslag_soort == 220)   { $row->field_verslag_soort = 40;}
	elseif ($row->field_verslag_soort == 202)   { $row->field_verslag_soort = 60;}
	elseif ($row->field_verslag_soort == 218)   { $row->field_verslag_soort = 80;}
	elseif ($row->field_verslag_soort == 132)   { $row->field_verslag_soort = 50;}
	elseif ($row->field_verslag_soort == 130)   { $row->field_verslag_soort = 30;}
	
    return TRUE;
  }
  
}

class FilePartituurMigration extends DrupalFile6Migration {
  protected function query() {
    $query = parent::query();

    $query->leftJoin('content_field_partij_band','b','b.field_partij_band_fid=f.fid');
    $query->leftJoin('content_field_partij_koor_l','k','k.field_partij_koor_l_fid=f.fid');
    $query->leftJoin('content_field_partij_tekst','t','t.field_partij_tekst_fid=f.fid');
    $query->addField('b','nid','bnid');
    $query->addField('k','nid','knid');
    $query->addField('t','nid','tnid');
    $query->addField('b','field_partij_band_data');
    $query->addField('k','field_partij_koor_l_data');
    $query->addField('t','field_partij_tekst_data');
    $query->condition('f.filepath','%repertoire%','LIKE');
    return $query;

  }
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('preserve_files', NULL, FALSE)
      ->defaultValue(FALSE);
    $this->addFieldMapping('file_replace', NULL, FALSE)
      ->defaultValue(FILE_EXISTS_REPLACE);
      
    $this->addFieldMapping('field_partituur_soort', 'field_partituur_soort');
    $this->addFieldMapping('field_bestand_nummer', 'field_bestand_nummer')
         ->sourceMigration('Node_Repertoire');
    $this->addFieldMapping('field_toegang', 'field_toegang')
         ->sourceMigration('Term_Toegang');

  }
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    
    if ($row->bnid) { // Bandpartituur
      if (strpos($row->field_partij_band_data, 'Toetsen') !== false )    { $row->field_partituur_soort = '11'; }
      elseif (strpos($row->field_partij_band_data, 'Piano') !== false )  { $row->field_partituur_soort = '11'; }
      elseif (strpos($row->field_partij_band_data, 'Gitaar') !== false ) { $row->field_partituur_soort = '12'; }
      elseif (strpos($row->field_partij_band_data, 'Bas') !== false )    { $row->field_partituur_soort = '13'; }
      elseif (strpos($row->field_partij_band_data, 'Drums') !== false )  { $row->field_partituur_soort = '14'; }
      else {                                                               $row->field_partituur_soort = '10'; }
      $row->field_bestand_nummer = $row->bnid;
      $row->field_toegang = array(217, 86, 85);
    }
    elseif ($row->knid) { // Koorpartituur
      if (strpos($row->field_partij_koor_l_data, 'MuseScore') !== false )    { $row->field_partituur_soort = '2'; }
      else {                                                                   $row->field_partituur_soort = '1'; }
      $row->field_bestand_nummer = $row->knid;
      $row->field_toegang = array(217, 86, 28, 85);
    }
    elseif ($row->tnid) { // Tekst of koorregie
      if (strpos($row->field_partij_tekst_data, 'Tekst en koorregie') !== false )    { $row->field_partituur_soort = '5'; }
      elseif (strpos($row->field_partij_tekst_data, 'Koorregie') !== false )         { $row->field_partituur_soort = '4'; }
      else {                                                                           $row->field_partituur_soort = '3'; }
      $row->field_bestand_nummer = $row->tnid;
      $row->field_toegang = array(217, 86, 28, 85);
    }
    
    return TRUE;
  }
}

class FileImageMigration extends DrupalFile6Migration {
  protected function query() {
    $query = parent::query();

    $query->condition('f.filemime',array('image/jpeg','image/png','image/gif'),'IN');

    return $query;
  }
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('preserve_files', NULL, FALSE)
      ->defaultValue(FALSE);
    $this->addFieldMapping('file_replace', NULL, FALSE)
      ->defaultValue(FILE_EXISTS_REPLACE);

  }
}

class FileAudioMigration extends DrupalFile6Migration {
  protected function query() {
    $query = parent::query();

    $query->leftJoin('content_type_audio','n','n.field_mp3_fid=f.fid');
    $query->leftJoin('content_field_audio_type','t','t.nid=n.nid AND t.vid=n.vid');
    $query->leftJoin('content_field_audio_uitvoerende','u','u.nid=n.nid AND u.vid=n.vid');
    $query->leftJoin('content_field_datum','d','d.nid=n.nid AND d.vid=n.vid');
    $query->leftJoin('content_field_ref_activiteit','a','a.nid=n.nid AND a.vid=n.vid');
    $query->leftJoin('content_field_repertoire','r','r.nid=n.nid AND r.vid=n.vid');
    $query->addField('t','field_audio_type_value','field_media_type');
    $query->addField('t','nid','nid');
    $query->addField('t','vid','vid');
    $query->addField('n','field_audio_bijz_value','field_media_bijzonderheden');
    $query->addField('u','field_audio_uitvoerende_value','field_media_uitvoerende');
    $query->addField('d','field_datum_value','field_media_datum');
    $query->addField('a','field_ref_activiteit_nid','field_media_activiteit');
    $query->addField('r','field_repertoire_nid','field_bestand_nummer');
    $query->condition('f.filemime','audio/mpeg');

    return $query;
  }
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('preserve_files', NULL, FALSE)
      ->defaultValue(FALSE);
    $this->addFieldMapping('file_replace', NULL, FALSE)
      ->defaultValue(FILE_EXISTS_REPLACE);
      
    $this->addFieldMapping('field_media_type', 'field_media_type');
    $this->addFieldMapping('field_media_bijzonderheden', 'field_media_bijzonderheden');
    $this->addFieldMapping('field_media_uitvoerende', 'field_media_uitvoerende');
    $this->addFieldMapping('field_media_datum', 'field_media_datum');
    $this->addFieldMapping('field_media_activiteit', 'field_media_activiteit')
         ->sourceMigration('Node_Activiteiten');
    $this->addFieldMapping('field_bestand_nummer', 'field_bestand_nummer')
         ->sourceMigration('Node_Repertoire');
    $this->addFieldMapping('field_toegang', 'field_toegang')
         ->sourceMigration('Term_Toegang');

  }
  
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    $term = array();
    $term_query = Database::getConnection('default', $this->sourceConnection)
      ->select('term_node', 't')
      ->fields('t')
      ->condition('t.nid',$row->nid)
      ->condition('t.vid',$row->vid);
    $term_query->leftJoin('term_data','td','td.tid=t.tid');
    $term_query->condition('td.vid',4);
    $term_result = $term_query->execute();
    foreach ($term_result as $term_record) {
      $term[] = $term_record->tid;
//      $message = 'NID ' . $row->nid . ' TID ' . $term_record->tid;
//      $this->queueMessage($message);
    }
    $row->field_toegang = $term;
	
    return TRUE;
  }
}