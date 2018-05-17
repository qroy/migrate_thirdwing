<?php

class FileDocMigration extends DrupalFile6Migration {
  protected function query() {
    $query = parent::query();
  
    $query->condition('f.filemime','%image%','NOT LIKE');
    $query->condition('f.filemime','%audio%','NOT LIKE');
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
    
    $row->field_toegang = thirdwing_migrate_returnterms($row->nid,$row->vid,4,$this->sourceConnection);
    
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
    $query->leftJoin('content_field_repertoire','r','r.nid=n.nid AND r.vid=n.vid');
    $query->addField('t','field_audio_type_value','field_media_type');
    $query->addField('t','nid','nid');
    $query->addField('t','vid','vid');
    $query->addField('n','field_audio_bijz_value','field_media_bijzonderheden');
    $query->addField('u','field_audio_uitvoerende_value','field_media_uitvoerende');
    $query->addField('d','field_datum_value','field_media_datum');
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
    
    $row->field_toegang = thirdwing_migrate_returnterms($row->nid,$row->vid,4,$this->sourceConnection);
    $row->field_media_activiteit = thirdwing_migrate_activiteitenrefs($row->nid,$row->vid,$this->sourceConnection);
    
    if ($row->field_media_type == 'Uitvoering')       { $row->field_media_type = 1;}
    elseif ($row->field_media_type == 'Repetitie')    { $row->field_media_type = 2;}
    elseif ($row->field_media_type == 'Oefenbestand') { $row->field_media_type = 3;}
    elseif ($row->field_media_type == 'Origineel')    { $row->field_media_type = 4;}
    elseif ($row->field_media_type == 'Uitzending')   { $row->field_media_type = 5;}
    elseif ($row->field_media_type == 'Overig')       { $row->field_media_type = 100;}
	
    return TRUE;
  }
}

class FileVideoMigration extends DrupalMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);
    if (!empty($arguments['user_migration'])) {
       $user_migration = $arguments['user_migration'];
       $this->dependencies[] = $user_migration;
    }
    $this->description = t('Migration of Youtube videos');
    $this->sourceType = $arguments['source_type'];
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'uri' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'YouTube URI',
        )
      ),
      MigrateDestinationMedia::getKeySchema()
    );
    
    if (!$this->newOnly) {
      $this->highwaterField = array(
        'name' => 'changed',
        'alias' => 'n',
        'type' => 'int',
      );
    }
    
    // Source fields available.
    $this->sourceFields = array(
      'uri' => t('URI of a YouTube video'),
      'nid' => t('Node ID'),
      'vid' => t('Node revision ID'),
      'uid' => t('Authored by (uid)'),
      'changed' => t('Modified timestamp'),
      'field_media_uitvoerende' => t('Uitvoerende artiest'),
      'field_media_datum' => t('Datum'),
      'field_media_activiteit' => t('Gerelateerde activiteiten'),
      'field_media_type' => t('Type media'),
      'field_bestand_nummer' => t('Gerelateerd repertoire'),
      'field_toegang' => t('Toegang term'),
    );
    
    $this->source = new MigrateSourceSQL($this->query(),
      $this->sourceFields, NULL, $this->sourceOptions);
    
    $this->destination = new MigrateDestinationFile($arguments['bundle'],'MyCustomMigrateFileYoutube');
   
    $this->addFieldMapping('value', 'uri');
    $this->addFieldMapping('field_media_type', 'field_media_type');
    $this->addFieldMapping('field_media_uitvoerende', 'field_media_uitvoerende');
    $this->addFieldMapping('field_media_datum', 'field_media_datum');
    $this->addFieldMapping('field_media_activiteit', 'field_media_activiteit')
         ->sourceMigration('Node_Activiteiten');
    $this->addFieldMapping('field_bestand_nummer', 'field_bestand_nummer')
         ->sourceMigration('Node_Repertoire');
    $this->addFieldMapping('field_toegang', 'field_toegang')
         ->sourceMigration('Term_Toegang');

    if (isset($arguments['default_uid'])) {
      $default_uid = $arguments['default_uid'];
    }
    else {
      $default_uid = 1;
    }
    if (isset($user_migration)) {
      $this->addFieldMapping('uid', 'uid')
           ->sourceMigration($user_migration)
           ->defaultValue($default_uid);
    }
    else {
      $this->addFieldMapping('uid')
           ->defaultValue($default_uid);
    }

    $this->addFieldMapping('timestamp', NULL, FALSE);

    $this->addUnmigratedSources(array('nid','vid','changed'));
    $this->addUnmigratedDestinations(array('field_media_bijzonderheden', 'path','field_media_datum:timezone','field_media_datum:rrule','field_media_datum:to'));
  }
  
    /**
   * Implements DrupalMigration::query().
   *
   * @return QueryConditionInterface|SelectQueryInterface
   */
  protected function query() {
    $query = Database::getConnection('default', $this->sourceConnection)
             ->select('node', 'n')
             ->fields('n', array('nid', 'vid', 'uid', 'changed'))
             ->condition('n.type', $this->sourceType)
             ->orderBy($this->newOnly ? 'n.nid' : 'n.changed');
    $query->innerJoin('content_field_video','v','v.nid=n.nid AND v.vid=n.vid');
    $query->leftJoin('content_field_audio_type','t','t.nid=n.nid AND t.vid=n.vid');
    $query->leftJoin('content_field_audio_uitvoerende','u','u.nid=n.nid AND u.vid=n.vid');
    $query->leftJoin('content_field_datum','d','d.nid=n.nid AND d.vid=n.vid');
    $query->leftJoin('content_field_repertoire','r','r.nid=n.nid AND r.vid=n.vid');
    $query->addField('v','field_video_embed','uri');
    $query->addField('t','field_audio_type_value','field_media_type');
    $query->addField('u','field_audio_uitvoerende_value','field_media_uitvoerende');
    $query->addField('d','field_datum_value','field_media_datum');
    $query->addField('r','field_repertoire_nid','field_bestand_nummer');
    return $query;
  }
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
    
    $row->field_toegang = thirdwing_migrate_returnterms($row->nid,$row->vid,4,$this->sourceConnection);
    $row->field_media_activiteit = thirdwing_migrate_activiteitenrefs($row->nid,$row->vid,$this->sourceConnection);
    
    if ($row->field_media_type == 'Uitvoering')       { $row->field_media_type = 1;}
    elseif ($row->field_media_type == 'Repetitie')    { $row->field_media_type = 2;}
    elseif ($row->field_media_type == 'Oefenbestand') { $row->field_media_type = 3;}
    elseif ($row->field_media_type == 'Origineel')    { $row->field_media_type = 4;}
    elseif ($row->field_media_type == 'Uitzending')   { $row->field_media_type = 5;}
    elseif ($row->field_media_type == 'Overig')       { $row->field_media_type = 100;}
	
    return TRUE;
  }
}

/**
 * Custom Class extending MigrateExtrasFileYoutube to allow for file fields.
 */
class MyCustomMigrateFileYoutube extends MigrateExtrasFileYoutube {

  /**
   * __construct method proposed as part of patch in #2
   */
  public function __construct($arguments = array(), $default_file = NULL) {
    if ($default_file) {
      $this->defaultFile = $default_file;
    }
    else {
      $this->defaultFile = new stdClass;
    }
  }

  /**
   * Implementation of MigrateFileInterface::processFiles().
   *
   * @param $value
   *  The URI or local filespec of a file to be imported.
   * @param $owner
   *  User ID (uid) to be the owner of the file.
   * @return object
   *  The file entity being created or referenced.
   */
  public function processFile($value, $owner) {
    // Convert the Youtube URI into a local stream wrapper.
    if (class_exists('MediaInternetYouTubeHandler')) {
      $handler = new MediaInternetYouTubeHandler($value);
      $destination = $handler->parse($value);
      $file = file_uri_to_object($destination, TRUE);
    }
    elseif (class_exists('MediaInternetOEmbedHandler')) {
      $handler = new MediaInternetOEmbedHandler($value);
      $file = $handler->getFileObject();
    }
    else {
      MigrationBase::displayMessage(t('Could not find a handler for YouTube videos'));
      return FALSE;
    }

    /**
      * code to map the fields, proposed as part of patch in #2
      */
    // Both file_uri_to_object() and MediaInternetOEmbedHandler::getFileObject()
    // give us a brand new file object, whereas we have default values from
    // Migrate's field mappings that we want to include.
    $file_original_data = (array) $this->defaultFile;
    foreach ($file_original_data as $property => $value) {
      $file->{$property} = $value;
    }

    // Create a file entity object for this Youtube reference, and see if we
    // can get the video title.
    if (empty($file->fid) && $info = $handler->getOEmbed()) {
      $file->filename = truncate_utf8($info['title'], 255);
    }
    $file = file_save($file);
    if (is_object($file)) {
      return $file;
    }
    else {
      return FALSE;
    }
  }
}

function thirdwing_migrate_activiteitenrefs($nid, $vid, $sourceConnection) {

  $refs = array();
  $refs_query = Database::getConnection('default', $sourceConnection)
    ->select('content_field_ref_activiteit', 'a')
    ->fields('a')
    ->condition('a.nid',$nid)
    ->condition('a.vid',$vid);
  $refs_result = $refs_query->execute();
  foreach ($refs_result as $refs_record) {
    $refs[] = $refs_record->field_ref_activiteit_nid;
  }
  return $refs;

}

function thirdwing_migrate_returnterms($nid, $vid, $tvid, $sourceConnection) {

  $term = array();
  $term_query = Database::getConnection('default', $sourceConnection)
    ->select('term_node', 't')
    ->fields('t')
    ->condition('t.nid',$nid)
    ->condition('t.vid',$vid);
  $term_query->leftJoin('term_data','td','td.tid=t.tid');
  $term_query->condition('td.vid',$tvid);
  $term_result = $term_query->execute();
  foreach ($term_result as $term_record) {
    $term[] = $term_record->tid;
  }
  return $term;

}