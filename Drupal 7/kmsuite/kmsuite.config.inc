<?php

/**
 * @file
 * Knowledge Marketing Config File.
 */

/**
 * Form builder; Configure the knowledge marketing requirements.
 *
 * @see system_settings_form()
 */
function kmsuite_config_settings($form, &$form_state) {

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);

  $form['description'] = array(
    '#markup' => '<div>' . t('Welcome to the Knowledge Marketing API Configuration.  This module depends on the <a href=":suite">kmsuite</a> and also requires a token which can be obtained from contacting <a href=":kmcontact">Knowledge Marketing</a>', array('!suite' => url('google.com'), '!kmcontact' => url('google.com'))) . '</div>',
  );

  // API Token.
  $form['km_apitoken'] = array(
    '#type' => 'textfield',
    '#title' => t('API Token'),
    '#default_value' => $presets['config']['token'],
    '#description' => t('The API token is required to use this module.  You can contact support@knowledgemarketing.com for more information.'),
  );

  // Domain Tracking Fieldset.  The Domain Tracking tool will inject
  // Javascript into the header or footer, where KM can track the user.
  $form['km_domain_tracker'] = array(
    '#type' => 'fieldset',
    '#title' => t('Domain Tracking'),
    '#description' => t('Domain tracking is used by Knowledge Marketing to track the activity of your subscribers on the website using a Cookie of ECNEmailAddress.'),
    '#collapsible' => TRUE,
    '#collapsed' => $presets['domain_tracker']['status'] == 1 ? FALSE : TRUE,
  );
  $form['km_domain_tracker']['km_domain_tracker_status'] = array(
    '#type' => 'select',
    '#options' => array(
      0 => t('No'),
      1 => t('Yes'),
    ),
    '#title' => t('Enabled'),
    '#default_value' => $presets['domain_tracker']['status'] ? $presets['domain_tracker']['status'] : 0,
  );
  $form['km_domain_tracker']['km_domain_tracker_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Domain Tracking Token'),
    '#description' => t('You can find the domain tracker key under the domain tracking tab in the ECN.'),
    '#default_value' => $presets['domain_tracker']['key'] ? $presets['domain_tracker']['key'] : NULL,
  );
  $form['km_domain_tracker']['km_domain_tracker_excluded_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Enter additional pages which you would like to exclude the domain tracker.'),
    '#description' => t('Enter each page on a new line'),
    '#default_value' => $presets['domain_tracker']['excluded_pages'] ? $presets['domain_tracker']['excluded_pages'] : KMSUITE_KM_EXCLUDED_PAGES,
    '#rows' => 7,
  );
  $form['km_domain_tracker']['km_domain_tracker_local'] = array(
    '#type' => 'select',
    '#options' => array(
      'header' => t('Header'),
      'footer' => t('Footer'),
    ),
    '#title' => t('Select the default location for the Javascript Injection (preference in the footer).'),
    '#default_value' => $presets['domain_tracker']['location'] ? $presets['domain_tracker']['location'] : 'footer',
  );

  // Taxonomy Tracker.  This widget will track potential terms from a set of
  // vocabularies into the page.  Note:  We handle this with a block incase
  // the user wants to exclude from specific pages, users, etc.
  $form['km_taxo_tracker'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxonomy Tracking'),
    '#description' => t('Taxonomy Tracker allows you to select which terms from a specific set of vocabularies to track, and on which pages you wish to track them on. <br /> Note:  This will only be available on NODE pages.'),
    '#collapsible' => TRUE,
    '#collapsed' => $presets['taxonomy_tracker']['status'] == 1 ? FALSE : TRUE,
  );
  $form['km_taxo_tracker']['km_taxo_tracker_status'] = array(
    '#type' => 'select',
    '#options' => array(
      0 => t('No'),
      1 => t('Yes'),
    ),
    '#title' => t('Enabled'),
    '#default_value' => $presets['taxonomy_tracker']['status'] ? $presets['taxonomy_tracker']['status'] : 'footer',
  );

  $vocabularies = taxonomy_get_vocabularies();
  $vocabs = array();
  foreach ($vocabularies as $vocab) {
    $vocabs[$vocab->vid] = $vocab->name;
  }

  $form['km_taxo_tracker']['km_taxo_tracker_vocabs'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' => $vocabs,
    '#title' => t('Taxonomy Vocabularies Tracker'),
    '#description' => t('Select as many vocabularies as you would like to track.  When the page loads, we will draw up all terms from those vocabularies for you.'),
    '#default_value' => $presets['taxonomy_tracker']['vocabularies'] ? $presets['taxonomy_tracker']['vocabularies'] : '',
  );
  $form['km_taxo_tracker']['km_taxo_tracker_values'] = array(
    '#type' => 'select',
    '#options' => array(
      'tid' => t('tid'),
      'name' => t('name'),
    ),
    '#title' => t('Rendered Output'),
    '#description' => t('Choose between dispaying term tid or the term name for the rendered output<br />Example tid: 10, 20, 30<br />Example name: Weather, hot, cold'),
    '#default_value' => $presets['taxonomy_tracker']['type'] ? $presets['taxonomy_tracker']['type'] : 'name',
  );
  $form['km_taxo_tracker']['km_taxo_tracker_case'] = array(
    '#type' => 'select',
    '#options' => array(
      0 => t('None'),
      'strtoupper' => t('UPPER'),
      'strtolower' => t('lower'),
      'ucfirst' => t('Capitalize'),
    ),
    '#title' => t('Case Transformation'),
    '#description' => t('If you would like to transform the output case of the term, you can select to do so.'),
    '#default_value' => $presets['taxonomy_tracker']['case'] ? $presets['taxonomy_tracker']['case'] : 0,
  );
  $form['km_taxo_tracker']['km_taxo_tracker_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Prefix'),
    '#description' => t('If you would like to add an optional prefix, please add the value to the field.  If empty, no prefix will be added.<br />Example prefix: prefix-weather, prefix-cold, prefix-hot'),
    '#default_value' => $presets['taxonomy_tracker']['prefix'] ? $presets['taxonomy_tracker']['prefix'] : NULL,
  );
  $form['km_taxo_tracker']['km_taxo_tracker_tid_exclusion'] = array(
    '#type' => 'textarea',
    '#title' => t('Term Exclusion'),
    '#description' => t('Enter a comma separated value of TIDS you would like to EXCLUDE from the output.'),
    '#default_value' => $presets['taxonomy_tracker']['tid_exclusion'] ? $presets['taxonomy_tracker']['tid_exclusion'] : NULL,
    '#rows' => 3,
  );

  $form['#submit'][] = 'kmsuite_config_settings_submit';

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

/**
 * Knowledge Marketing Settings Submit.
 *
 * Custom handler required to turn form elements into object.
 */
function kmsuite_config_settings_submit($form, &$form_state) {

  $form_data = $form_state['values'];

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);

  // General Config.
  $presets['config']['token'] = $form_data['km_apitoken'];

  // Domain Tracker.
  $presets['domain_tracker']['status'] = $form_data['km_domain_tracker_status'];
  $presets['domain_tracker']['key'] = $form_data['km_domain_tracker_key'];
  $presets['domain_tracker']['location'] = $form_data['km_domain_tracker_local'];
  $presets['domain_tracker']['excluded_pages'] = $form_data['km_domain_tracker_excluded_pages'];

  // Taxonomy Tracker.
  $presets['taxonomy_tracker']['status'] = $form_data['km_taxo_tracker_status'];
  $presets['taxonomy_tracker']['vocabularies'] = $form_data['km_taxo_tracker_vocabs'];
  $presets['taxonomy_tracker']['type'] = $form_data['km_taxo_tracker_values'];
  $presets['taxonomy_tracker']['case'] = $form_data['km_taxo_tracker_case'];
  $presets['taxonomy_tracker']['prefix'] = $form_data['km_taxo_tracker_prefix'];
  $presets['taxonomy_tracker']['tid_exclusion'] = $form_data['km_taxo_tracker_tid_exclusion'];

  variable_set('kmsuite_object', json_encode($presets));
  drupal_set_message(t('Configuration settings saved.'), 'status', FALSE);
}
