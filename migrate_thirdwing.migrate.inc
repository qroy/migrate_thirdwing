<?php
/**
 * @file migrate_thirdwing_example.migrate.inc
 */


/**
 * Implements hook_migrate_api().
 */
function migrate_thirdwing_migrate_api() {
  $api = array(
    'api' => 2,
    'groups' => array(
      'content_migration' => array(
        'title' => t('Migratie van Thirdwing.nl D6 naar D7'),
      ),
    ),
  );

  // Specify the DB connection and the source Drupal version.
  $common_arguments = array(
    'source_connection' => 'legacy',
    'source_version' => 6,
    'group_name' => 'content_migration',
    'format_mappings' => array(
      '1' => 'filtered_html',
      '3' => 'full_html',
      '6' => 'wysiwyg',
    ),
  );

  // Userpictures
  $picture_arguments = $common_arguments + array(
    'machine_name' => 'Picture',
    'description' => t('Import Drupal 6 picture files'),
    'default_uid' => 1,
    'source_dir' => 'http://www.thirdwing.nl/',
    'destination_dir' => 'public://',
  );
  
  Migration::registerMigration('DrupalPicture6Migration', $picture_arguments['machine_name'], $picture_arguments);

  // Users
  $user_arguments = $common_arguments + array( 
    'description' => t('Migration of users from Drupal 6'), 
    'machine_name' => 'User',
    'role_migration' => 'Role',
    'picture_migration' => 'Picture',
    'soft_dependencies' => array('Picture'),
    'class_name' => 'ThirdwingUserMigration',
  );
  
  Migration::registerMigration('ThirdwingUserMigration', $user_arguments['machine_name'], $user_arguments);

  // Roles
  $role_arguments = $common_arguments + array(
    'machine_name' => 'Role',
    'description' => t('Import Drupal 6 roles'),
    'role_mappings' => array(
      'Admin' => 'Admin',
      'Auteur' => 'Auteur',
      'Band' => 'Band',
      'Beheerder' => 'Beheerder',
      'Bestuur' => 'Bestuur',
      'Commissie Concerten' => 'Commissie Concerten',
      'Commissie Interne Relaties' => 'Commissie Interne Relaties',
      'Commissie Koorregie' => 'Commissie Koorregie',
      'Commissie Publieke Relaties' => 'Commissie Publieke Relaties',
      'Feestcommissie' => 'Feestcommissie',
      'Lid' => 'Leden',
      'Muziekcommissie' => 'Muziekcommissie',
      'Vriend' => 'Vrienden',
    ),
  );

  Migration::registerMigration('DrupalRole6Migration', $role_arguments['machine_name'], $role_arguments);


  //Taxonomy
  $toegang_term_arguments = $common_arguments + array(
    'machine_name' => 'Term_Toegang',
    'description' => t('Import Drupal 6 toegang terms into drupal 7'),
    'source_vocabulary' => '4',  // "Toegang" vocabulary
    'destination_vocabulary' => 'toegang',
  );
  Migration::registerMigration('DrupalTerm6Migration', $toegang_term_arguments['machine_name'], $toegang_term_arguments);

  // Files documents
  $files_arguments = $common_arguments + array( 
    'description' => t('Migration of document files from Drupal 6'), 
    'machine_name' => 'File',
    'user_migration' => 'User',
    'default_uid' => 1,
    'source_dir' => 'http://www.thirdwing.nl/',
    'destination_dir' => 'public://',
    'bundle' => 'document',
  );

  Migration::registerMigration('FileDocMigration', $files_arguments['machine_name'], $files_arguments);
  
  // Files verslagen
  $verslag_arguments = $common_arguments + array( 
    'description' => t('Migration of verslagen files from Drupal 6'), 
    'machine_name' => 'Verslag',
    'user_migration' => 'User',
    'default_uid' => 1,
    'source_dir' => 'http://www.thirdwing.nl/',
    'destination_dir' => 'public://',
    'bundle' => 'verslag',
  );

  Migration::registerMigration('FileVerslagMigration', $verslag_arguments['machine_name'], $verslag_arguments);
  
  // Files partituren
  $partituur_arguments = $common_arguments + array( 
    'description' => t('Migration of partituur files from Drupal 6'), 
    'machine_name' => 'Partituur',
    'user_migration' => 'User',
    'default_uid' => 1,
    'source_dir' => 'http://www.thirdwing.nl/',
    'destination_dir' => 'public://',
    'bundle' => 'partituur',
    'dependencies' => array('Node_Repertoire', 'Term_Toegang'),
  );

  Migration::registerMigration('FilePartituurMigration', $partituur_arguments['machine_name'], $partituur_arguments);

 // Images
  $images_arguments = $common_arguments + array( 
    'description' => t('Migration of image files from Drupal 6'), 
    'machine_name' => 'Image',
    'user_migration' => 'User',
    'default_uid' => 1,
    'source_dir' => 'http://www.thirdwing.nl/',
    'destination_dir' => 'public://',
    'bundle' => 'image',
  );

  Migration::registerMigration('FileImageMigration', $images_arguments['machine_name'], $images_arguments);

 // Audio
  $audio_arguments = $common_arguments + array( 
    'description' => t('Migration of audio files from Drupal 6'), 
    'machine_name' => 'Audio',
    'user_migration' => 'User',
    'default_uid' => 1,
    'source_dir' => 'http://www.thirdwing.nl/',
    'destination_dir' => 'public://',
    'bundle' => 'audio',
    'dependencies' => array('Node_Activiteiten','Node_Repertoire'),
  );

  Migration::registerMigration('FileAudioMigration', $audio_arguments['machine_name'], $audio_arguments);
  
  // Video
  $video_arguments = $common_arguments + array( 
    'description' => t('Migration of video files from Drupal 6'), 
    'machine_name' => 'Video',
    'user_migration' => 'User',
    'default_uid' => 1,
    'source_dir' => 'http://www.thirdwing.nl/',
    'destination_dir' => 'youtube://',
    'source_type' => 'video',
    'bundle' => 'video',
    'dependencies' => array('Node_Activiteiten','Node_Repertoire'),
  );

  Migration::registerMigration('FileVideoMigration', $video_arguments['machine_name'], $video_arguments);
  

  // Menus
  $menu_arguments = $common_arguments + array(
    'machine_name' => 'Menu',
    'description' => t('Import Drupal 6 menus'),
  );

  Migration::registerMigration('DrupalMenu6Migration', $menu_arguments['machine_name'], $menu_arguments);

  // Menu links
  $menu_link_arguments = $common_arguments + array(
    'machine_name' => 'MenuLink',
    'description' => t('Import Drupal 6 menu links'),
    'menu_migration' => 'Menu',
    'node_migrations' => array('Node_Pagina','Node_Activiteiten','Node_Nieuws','Node_Repertoire','Node_Programma','Node_Locatie','Node_Album','Node_Vriend','Node_Nieuwsbrief'),
  );

  Migration::registerMigration('DrupalMenuLinks6Migration', $menu_link_arguments['machine_name'], $menu_link_arguments);

  // Nodes
  $node_arguments = $common_arguments + array(
    'user_migration' => 'User',
    'default_uid' => 1,
  );

  // Nodes - activiteiten
  $activiteiten_node_arguments = $node_arguments + array(
    'machine_name' => 'Node_Activiteiten',
    'description' => t('Import Drupal 6 activiteiten nodes'),
    'source_type' => 'activiteit', 
    'destination_type' => 'activiteit',
    'dependencies' => array('Term_Toegang','User','Node_Programma','Node_Repertoire','Node_Locatie','File','Image'),
  );

  Migration::registerMigration('ActiviteitenNodeMigration', $activiteiten_node_arguments['machine_name'], $activiteiten_node_arguments);

  // Nodes - programma
  $programma_node_arguments = $node_arguments + array(
    'machine_name' => 'Node_Programma',
    'description' => t('Import Drupal 6 programma nodes'),
    'source_type' => 'programma', 
    'destination_type' => 'repertoire',
    'dependencies' => array('User'),
  );

  Migration::registerMigration('ProgrammaNodeMigration', $programma_node_arguments['machine_name'], $programma_node_arguments);

  // Nodes - repertoire
  $repertoire_node_arguments = $node_arguments + array(
    'machine_name' => 'Node_Repertoire',
    'description' => t('Import Drupal 6 repertoire nodes'),
    'source_type' => 'repertoire', 
    'destination_type' => 'repertoire',
    'dependencies' => array('User'),
  );

  Migration::registerMigration('RepertoireNodeMigration', $repertoire_node_arguments['machine_name'], $repertoire_node_arguments);

  // Nodes - nieuws
  $nieuws_node_arguments = $node_arguments + array(
    'machine_name' => 'Node_Nieuws',
    'description' => t('Import Drupal 6 nieuws nodes'),
    'source_type' => 'nieuws', 
    'destination_type' => 'nieuws',
    'dependencies' => array('User','Term_Toegang','File','Image'),
  );

  Migration::registerMigration('NieuwsNodeMigration', $nieuws_node_arguments['machine_name'], $nieuws_node_arguments);

  // Nodes - pagina
  $pagina_node_arguments = $node_arguments + array(
    'machine_name' => 'Node_Pagina',
    'description' => t('Import Drupal 6 pagina nodes'),
    'source_type' => 'pagina', 
    'destination_type' => 'pagina',
    'dependencies' => array('User','Term_Toegang','File','Image'),
  );

  Migration::registerMigration('PaginaNodeMigration', $pagina_node_arguments['machine_name'], $pagina_node_arguments);

  // Nodes - locatie
  $locatie_node_arguments = $node_arguments + array(
    'machine_name' => 'Node_Locatie',
    'description' => t('Import Drupal 6 locatie nodes'),
    'source_type' => 'locatie', 
    'destination_type' => 'locatie',
    'dependencies' => array('User'),
  );

  Migration::registerMigration('LocatieNodeMigration', $locatie_node_arguments['machine_name'], $locatie_node_arguments);

  // Nodes - foto
  $album_node_arguments = $node_arguments + array(
    'machine_name' => 'Node_Album',
    'description' => t('Import Drupal 6 Foto nodes into albums'),
    'source_type' => 'foto', 
    'destination_type' => 'album',
    'dependencies' => array('User','File','Node_Activiteiten','Term_Toegang','Image'),
  );

  Migration::registerMigration('AlbumNodeMigration', $album_node_arguments['machine_name'], $album_node_arguments);

  // Nodes - vriend
  $vriend_node_arguments = $node_arguments + array(
    'machine_name' => 'Node_Vriend',
    'description' => t('Import Drupal 6 Vriend nodes'),
    'source_type' => 'vriend', 
    'destination_type' => 'vriend',
    'dependencies' => array('User','File','Image'),
  );

  Migration::registerMigration('VriendNodeMigration', $vriend_node_arguments['machine_name'], $vriend_node_arguments);

 // Nodes - nieuwsbrief
  $nieuwsbrief_node_arguments = $node_arguments + array(
    'machine_name' => 'Node_Nieuwsbrief',
    'description' => t('Import Drupal 6 Nieuwsbrief nodes'),
    'source_type' => 'nieuwsbrief', 
    'destination_type' => 'nieuwsbrief',
    'dependencies' => array('User','Node_Nieuws'),
  );

  Migration::registerMigration('NieuwsbriefNodeMigration', $nieuwsbrief_node_arguments['machine_name'], $nieuwsbrief_node_arguments);

  return $api;
}

?>