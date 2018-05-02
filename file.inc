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
      'description' => t('Description of the video'),
      'nid' => t('Node ID'),
      'title' => t('Title'),  // @todo Depends on has_title, and label may be customized
      'body' => t('Body'),
      'format' => t('Format'),
      'teaser' => t('Teaser'),
      'uid' => t('Authored by (uid)'),
      'created' => t('Created timestamp'),
      'changed' => t('Modified timestamp'),
      'status' => t('Published'),
      'promote' => t('Promoted to front page'),
      'sticky' => t('Sticky at top of lists'),
      'revision' => t('Create new revision'),
      'log' => t('Revision Log message'),
      'language' => t('Language (fr, en, ...)'),
      'tnid' => t('The translation set id for this node'),
      'revision_uid' => t('Revision modified by author (uid)'),
      'field_media_uitvoerende' => t('Uitvoerende artiest'),
      'field_media_datum' => t('Datum'),
      'field_media_activiteit' => t('Gerelateerde activiteiten'),
      'field_media_type' => t('Type media'),
      'field_bestand_nummer' => t('Gerelateerd repertoire'),
      'field_toegang' => t('Toegang term'),
    );
    
    $this->source = new MigrateSourceSQL($this->query(),
      $this->sourceFields, NULL, $this->sourceOptions);
    
    // In this case, we need to specify the file_class in the second paramter -
    // this class understands how to translate a http://www.youtube.com/ URI
    // into Drupal's Youtube file scheme (youtube://).
    $this->destination = new MigrateDestinationMedia('video',
      'MigrateExtrasFileYoutube');
      
    // We just need to map the 'value' in the media destination to the Youtube
    // URI.
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
  }
  
    /**
   * Implements DrupalMigration::query().
   *
   * @return QueryConditionInterface|SelectQueryInterface
   */
  protected function query() {
    $query = Database::getConnection('default', $this->sourceConnection)
             ->select('node', 'n')
             ->fields('n', array('nid', 'vid', 'language', 'title',
                 'uid', 'status', 'created', 'changed', 'comment', 'promote',
                 'moderate', 'sticky', 'tnid', 'translate'))
             ->condition('n.type', $this->sourceType)
             ->orderBy($this->newOnly ? 'n.nid' : 'n.changed');
    $query->innerJoin('node_revisions', 'nr', 'n.vid=nr.vid');
    $query->innerJoin('content_field_video','v','v.nid=n.nid AND v.vid=n.vid');
    $query->leftJoin('content_field_audio_type','t','t.nid=n.nid AND t.vid=n.vid');
    $query->leftJoin('content_field_audio_uitvoerende','u','u.nid=n.nid AND u.vid=n.vid');
    $query->leftJoin('content_field_datum','d','d.nid=n.nid AND d.vid=n.vid');
    $query->leftJoin('content_field_ref_activiteit','a','a.nid=n.nid AND a.vid=n.vid');
    $query->leftJoin('content_field_repertoire','r','r.nid=n.nid AND r.vid=n.vid');
    $query->fields('nr', array('body', 'teaser', 'format'));
    $query->addField('v','field_video_embed','uri');
    $query->addField('v','field_video_description','description');
    $query->addField('u','field_audio_uitvoerende_value','field_media_uitvoerende');
    $query->addField('d','field_datum_value','field_media_datum');
    $query->addField('a','field_ref_activiteit_nid','field_media_activiteit');
    $query->addField('r','field_repertoire_nid','field_bestand_nummer');
    return $query;
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
    }
    $row->field_toegang = $term;
	
    return TRUE;
  }
}
