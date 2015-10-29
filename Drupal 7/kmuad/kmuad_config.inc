<?php

/**
 * @file
 * Knowledge Marketing UAD Module  - UAD API key config.
 */

/**
 * Custom field settings for the Fields.
 */
function kmuad_settings($form, &$form_state) {

  $presets = json_decode(variable_get('kmuad_object'), TRUE);

  // print_r($presets);

  $schema_email = isset($presets['uad']['email']) ? $presets['uad']['email'] : '';
  $uadkey = isset($presets['uad']['key']) ? $presets['uad']['key'] : '';

  $test_email = isset($_COOKIE['ECNEmailAddress']) ? $_COOKIE['ECNEmailAddress'] : '';

  $form['km_uad'] = array(
    '#type' => 'fieldset',
    '#title' => t('UAD Configuration'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('The UAD plugin enabled your website to communicate directly with the UAD database.  This will bypass the ECN, and grab consensus subcription data.<br /><br />Note: test page created which you can see your demographics in action at: /uad-test'),
  );

  $form['km_uad']['uad_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#default_value' => $uadkey,
    '#description' => t('UAD Key:  Please contact technical support at Knowledge Marketing for this (it is different from your ECN API token)'),
  );

  $form['km_uad']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Schema Email'),
    '#default_value' => $schema_email,
    '#description' => t('Email addresses associated with the scheme you are looking to grab fields from.  Likely an administrators email address.'),
  );

  $form['km_uad_test'] = array(
    '#type' => 'fieldset',
    '#title' => t('Test Cookie Generation'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('By default, the website will look for the ECNEmaiAddress cookie for end users.  However, if we cannot find one, you can generate this cookie by inputting an email below and hitting generate.  This is useful for additional testing as well, so you can overwrite the cookie vs. using a 3rd party app to edit, or delete and reset.'),
    '#prefix' => '<div id="uad-config">',
    '#suffix' => '</div>'
  );

  $form['km_uad_test']['temail'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#default_value' => $test_email,
  );

  $form['km_uad_test']['generate'] = array(
    '#type' => 'submit',
    '#value' => 'Generate',
    '#submit' => array('ajax_kmuad_test_callback'),
    '#ajax' => array(
      'callback' => 'ajax_refresh_uad_test',
      'wrapper' => 'uad-config',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save Configuration',
    '#submit' => array('kmuad_settings_submit'),
  );

  return $form;
}

/**
 * Implimentation hook_submit.
 *
 * @param array $form
 *   description
 * @param array $form_state
 *   description
 *
 * @return array
 *   description
 */
function kmuad_settings_submit($form, &$form_state) {

  $values = $form_state['values'];
  $presets = json_decode(variable_get('kmuad_object'), TRUE);

  $presets['uad']['key'] = $values['uad_key'];
  $presets['uad']['email'] = $values['email'];

  variable_set('kmuad_object', json_encode($presets));

  drupal_flush_all_caches();
  drupal_set_message(t('Configuration settings saved.'), 'status', FALSE);
}

/**
 * Implements ajax callback.
 *
 * @return form
 *   return field
 */
function ajax_kmuad_test_callback($form, &$form_state) {

  $email = $form_state['values']['temail'];
  setrawcookie('ECNEmailAddress', ($email), REQUEST_TIME + 31536000, '/');
  setrawcookie('ECNDemographics', '', REQUEST_TIME - 3600, '/');
  get_demographics($email);
  $form_state['rebuild'] = TRUE;

  drupal_set_message(t('Set the cookie!'), 'status', FALSE);
}

/**
 * Implements callback.
 */
function ajax_refresh_uad_test($form, $form_state) {

  return $form['km_uad_test'];
}
