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
      'Commissie Faciliteiten' => 'Commissie Publieke Relaties',
      'Commissie Interne Relaties' => 'Commissie Interne Relaties',
      'Commissie Koorregie' => 'Commissie Koorregie',
      'Concertcommissie' => 'Concertcommissie',
      'Lid' => 'Leden',
      'Muziekcommissie' => 'Muziekcommissie',
      'Vriend' => 'Vrienden',
    ),
  );

  Migration::registerMigration('DrupalRole6Migration', $role_arguments['machine_name'], $role_arguments);


  //Taxonomy
  $toegang_term_arguments = $common_arguments + array(
    'machine_name' => 'ToegangTerm',
    'description' => t('Import Drupal 6 toegang terms into drupal 7'),
    'source_vocabulary' => '4',  // "Photo category" vocabulary
    'destination_vocabulary' => 'toegang',
  );
  Migration::registerMigration('DrupalTerm6Migration', $toegang_term_arguments['machine_name'], $toegang_term_arguments);

  // Files
  $files_arguments = $common_arguments + array( 
    'description' => t('Migration of files from Drupal 6'), 
    'machine_name' => 'File',
    'user_migration' => 'User',
    'default_uid' => 1,
    'source_dir' => 'http://www.thirdwing.nl/',
    'destination_dir' => 'public://',
  );

  Migration::registerMigration('DrupalFile6Migration', $files_arguments['machine_name'], $files_arguments);

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
    'node_migrations' => array('PaginaNode'),
  );

  Migration::registerMigration('DrupalMenuLinks6Migration', $menu_link_arguments['machine_name'], $menu_link_arguments);

  // Nodes
  $node_arguments = $common_arguments + array(
    'user_migration' => 'User',
    'default_uid' => 1,
  );

  // Nodes - activiteiten
  $activiteiten_node_arguments = $node_arguments + array(
    'machine_name' => 'ActiviteitenNode',
    'description' => t('Import Drupal 6 activiteiten nodes'),
    'source_type' => 'activiteit', 
    'destination_type' => 'activiteit',
    'dependencies' => array('ToegangTerm','User','ProgrammaNode','RepertoireNode','LocatieNode','File'),
  );

  Migration::registerMigration('ActiviteitenNodeMigration', $activiteiten_node_arguments['machine_name'], $activiteiten_node_arguments);

  // Nodes - programma
  $programma_node_arguments = $node_arguments + array(
    'machine_name' => 'ProgrammaNode',
    'description' => t('Import Drupal 6 programma nodes'),
    'source_type' => 'programma', 
    'destination_type' => 'repertoire',
    'dependencies' => array('User'),
  );

  Migration::registerMigration('ProgrammaNodeMigration', $programma_node_arguments['machine_name'], $programma_node_arguments);

  // Nodes - repertoire
  $repertoire_node_arguments = $node_arguments + array(
    'machine_name' => 'RepertoireNode',
    'description' => t('Import Drupal 6 repertoire nodes'),
    'source_type' => 'repertoire', 
    'destination_type' => 'repertoire',
    'dependencies' => array('User'),
  );

  Migration::registerMigration('RepertoireNodeMigration', $repertoire_node_arguments['machine_name'], $repertoire_node_arguments);

  // Nodes - nieuws
  $nieuws_node_arguments = $node_arguments + array(
    'machine_name' => 'NieuwsNode',
    'description' => t('Import Drupal 6 nieuws nodes'),
    'source_type' => 'nieuws', 
    'destination_type' => 'nieuws',
    'dependencies' => array('User','ToegangTerm','File'),
  );

  Migration::registerMigration('DrupalNode6Migration', $nieuws_node_arguments['machine_name'], $nieuws_node_arguments);

  // Nodes - pagina
  $pagina_node_arguments = $node_arguments + array(
    'machine_name' => 'PaginaNode',
    'description' => t('Import Drupal 6 pagina nodes'),
    'source_type' => 'pagina', 
    'destination_type' => 'pagina',
    'dependencies' => array('User','ToegangTerm','File'),
  );

  Migration::registerMigration('PaginaNodeMigration', $pagina_node_arguments['machine_name'], $pagina_node_arguments);

  // Nodes - locatie
  $locatie_node_arguments = $node_arguments + array(
    'machine_name' => 'LocatieNode',
    'description' => t('Import Drupal 6 locatie nodes'),
    'source_type' => 'locatie', 
    'destination_type' => 'locatie',
    'dependencies' => array('User'),
  );

  Migration::registerMigration('LocatieNodeMigration', $locatie_node_arguments['machine_name'], $locatie_node_arguments);

  // Nodes - album
  $album_node_arguments = $node_arguments + array(
    'machine_name' => 'AlbumNode',
    'description' => t('Import Drupal 6 album nodes'),
    'source_type' => 'album', 
    'destination_type' => 'album',
    'dependencies' => array('User','File','ActiviteitenNode','ToegangTerm'),
  );

  Migration::registerMigration('AlbumNodeMigration', $album_node_arguments['machine_name'], $album_node_arguments);

  return $api;
}

?>