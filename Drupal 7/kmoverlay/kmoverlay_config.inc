<?php
/**
 * @file
 * Knowledge Marketing Config Module File.
 */

/**
 * Implements hook_settings().
 */
function kmsuite_overlay_settings($form, $form_state) {

  $presets = json_decode(variable_get('kmsuite_object'), TRUE);
  
  //echo "<Pre>"; print_r($presets); die();
  

  $cookie_period = array(
    1 => t('1 Day'),
    7 => t('1 Week'),
    14 => t('2 Weeks'),
    30 => t('1 Month'),
    90 => t('3 Months'),
  );
  $page_delay = array(
    0 => 0,
    1 => 1,
    2 => 2,
    3 => 3,
    4 => 4,
    5 => 5,
  );

  $active = array('1' => 'Active', '0' => 'Inactive');

  $form['#tree'] = TRUE;

  $form['description'] = array(
    '#markup' => '<div>' . t('Subscription Overlay:  This module provides you the ability to present a form in an overlay with custom options (such as when to show the popup).  The conditions for this are determined in the configuration of the module.') . '</div>',
  );

  $form['overlay_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Overlay Configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['overlay_config']['status'] = array(
    '#type' => 'checkbox',
    '#title' => 'Overlay Status',
    '#description' => t('Turn the overlay on or off.  Note:  If developer options are enabled, this settings will be overridden.'),
    '#default_value' => isset($presets['overlay_plugin']['status'])? $presets['overlay_plugin']['status']: '',
  );
  $form['overlay_config']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Overlay Title'),
    '#description' => t('A custom title presented in the overlay box'),
    '#default_value' => isset($presets['overlay_plugin']['title'])? $presets['overlay_plugin']['title'] : 'Subscribe',
  );
  $form['overlay_config']['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Success Message'),
    '#description' => t('This message is shown after they submit the form.  Note:  It will only show if the redirect option is not enabled.'),
    '#default_value' => isset($presets['overlay_plugin']['message']) ? $presets['overlay_plugin']['message'] : t('Thank you for subscribing!'),
  );
  $form['overlay_config']['cookie_interval'] = array(
    '#type' => 'select',
    '#title' => t('Cookie Expiration Date'),
    '#description' => t('After the cookie has been set, we will not show the popup until X amount of time after.'),
    '#options' => $cookie_period,
    '#default_value' => isset($presets['overlay_plugin']['cookie_interval']) ? $presets['overlay_plugin']['cookie_interval'] : 7,
  );
  $form['overlay_config']['page_delay'] = array(
    '#type' => 'select',
    '#title' => t('Number of pages to delay popup'),
    '#description' => t('Often you may not want to hit your visitors up with a popup right away.  This method allows you to delay the popup for X page views.'),
    '#options' => $page_delay,
    '#default_value' => isset($presets['overlay_plugin']['page_delay'])? $presets['overlay_plugin']['page_delay'] : 0,
  );
  $form['overlay_config']['pester'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pester Visitor'),
    '#description' => t('Selecting this option will negate the close button cookie and pester the user with a popup every number of pages determined from the delay.  For example:  Instead of showing the popup on the second page and not again for X number of days, we now show the popup every X number of pages (until the give in and fill out the form).'),
    '#default_value' => isset($presets['overlay_plugin']['pester']) ? $presets['overlay_plugin']['pester'] : 0,
  );

  $form['overlay_config']['developer_options'] = array(
    '#type' => 'checkbox',
    '#title' => 'Developer Mode',
    '#description' => t('Turning this on will enable the popup on every page for testing purposes.'),
    '#default_value' => isset($presets['overlay_plugin']['developer_options']) ? $presets['overlay_plugin']['developer_options'] : 0,
  );

  $form['overlay_config']['redirect_status'] = array(
    '#type' => 'checkbox',
    '#title' => 'Redirect',
    '#description' => t('After the user has filled out the form, they will be redirected to the page indicated below.'),
    '#default_value' => isset($presets['overlay_plugin']['redirect_status'])?$presets['overlay_plugin']['redirect_status']:'',
  );
  $form['overlay_config']['redirect_page'] = array(
    '#type' => 'textfield',
    '#title' => 'Redirect Page',
    '#description' => t('Indicate the page you would like the subscriber to be redirected too.  Note:  By default we redirect them to the subscriber langing page with the email address as a parameter.'),
    '#default_value' => isset($presets['overlay_plugin']['redirect_page']) ? $presets['overlay_plugin']['redirect_page'] : 'subscription-management',
  );

  $form['excluded_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages not to allow'),
    '#description' => t('Specify pages by using their paths. Enter one path per line.'),
    '#default_value' => isset($presets['overlay_plugin']['excluded_pages']) ? $presets['overlay_plugin']['excluded_pages'] : KMSUITE_KM_EXCLUDED_PAGES,
  );

  $form['copy_config']['copy'] = array(
    '#type' => 'text_format',
    '#base_type' => 'textarea',
    '#format' => isset($settings['copy_config']['copy']['format']) ? $settings['copy_config']['copy']['format'] : 'filtered_html',
    '#title' => t('Overlay Description'),
    '#default_value' => isset($settings['copy_config']['copy']['value']) ? $settings['copy_config']['copy']['value'] : NULL,
  );

  $rows = array();
  $row_elements = array();

  $form['field_table'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field Selection'),
    '#description' => t('Optional fields are coming from the Field tab in the Subscription plugin.'),
  );

  $data = array();
  if (isset($presets['subscription_plugin']['groups'])) {
    foreach ($presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['fields'] as $k => $v) {

      $data[$k] = array(
          'enable' => isset($presets['overlay_plugin']['field_table'][$k]['enabled']) ? $presets['overlay_plugin']['field_table'][$k]['enabled'] : FALSE,
          'required' => isset($presets['overlay_plugin']['field_table'][$k]['required']) ? $presets['overlay_plugin']['field_table'][$k]['required'] : FALSE,
          'weight' => isset($presets['overlay_plugin']['field_table'][$k]['weight']) ? $presets['overlay_plugin']['field_table'][$k]['weight'] : 0,
          'name' => $presets['subscription_plugin']['fields']['kmsubscription_saved_fields']['names'][$k],
        );
    }

    // Sort the rows.
    uasort($data, 'array_uasort');

    // Build the rows.
    foreach ($data as $id => $entry) {
      // Build the table rows.
      $rows[$id] = array(
        'data' => array(
          // Cell for the cross drag&drop element.
          array('class' => array('entry-cross')),
          // Weight item for the tabledrag.
          array(
            'data' => array(
              '#type' => 'weight',
              '#title' => t('Weight'),
              '#title_display' => 'invisible',
              '#default_value' => $entry['weight'],
              '#parents' => array('field_table', $id, 'weight'),
              '#attributes' => array(
                'class' => array('field-order-weight'),
              ),
            )),
          // Enabled checkbox.
          array(
            'data' => array(
              '#type' => 'checkbox',
              '#title' => t('Enable'),
              '#title_display' => 'invisible',
              '#default_value' => $entry['enable'],
              '#parents' => array('field_table', $id, 'enabled'),
            )),
          // Enabled checkbox.
          array(
            'data' => array(
              '#type' => 'checkbox',
              '#title' => t('Required'),
              '#title_display' => 'invisible',
              '#default_value' => $entry['required'],
              '#parents' => array('field_table', $id, 'required'),
            )),

          check_plain($entry['name']),

        ),
        'class' => array('draggable'),
      );
      // Build rows of the form elements in the table.
      $row_elements[$id] = array(
        'weight' => &$rows[$id]['data'][1]['data'],
        'enabled' => &$rows[$id]['data'][2]['data'],
        'required' => &$rows[$id]['data'][3]['data'],
      );
    }

    // Add the table to the form.
    $form['field_table']['table'] = array(
      '#theme' => 'table',
      // The row form elements need to be processed and build,
      // therefore pass them as element children.
      'elements' => $row_elements,
      '#header' => array(
        // We need two empty columns for the weigth field and the cross.
        array('data' => NULL, 'colspan' => 2),
        t('Enabled'),
        t('Required'),
        t('Name'),
      ),
      '#rows' => $rows,
      '#empty' => t('There are no groups available.'),
      '#attributes' => array('id' => 'field-order'),
    );
    drupal_add_tabledrag('field-order', 'order', 'sibling', 'field-order-weight');
  }

  $saved_groups = $presets['subscription_plugin']['groups'];

  $saved_groups_alias = array();
  foreach ($saved_groups as $k => $v) {
    $saved_groups_alias[$k] = strlen($v['alias']) > 1 ? $v['alias'] : $v['origname'];
  }

  /* echo "<pre>"; print_r($presets['overlay_plugin']['overlay_groups']); die(); */
  $form['overlay_groups'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Overlay Group Selection'),
    '#description' => t('Select the group(s) you would like to subscriber to be put on when they fill out the form.  Note: The group management determines the available groups for this section.'),
    '#options' => $saved_groups_alias,
    '#default_value' => isset($presets['overlay_plugin']['overlay_groups']) ? $presets['overlay_plugin']['overlay_groups'] : array(),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
    '#submit' => array('kmoverlay_settings_submit'),
    '#prefix' => '<div>',
    '#suffix' => '</div>',
  );

  return $form;
}

/**
 * Submit Handler for the form.
 */
function kmoverlay_settings_submit($form, &$form_state) {
  $presets = json_decode(variable_get('kmsuite_object'), TRUE);

  //echo "<pre>"; print_r($presets); die();
  //echo "<Pre>"; print_r($form_state['values']); die();

  $presets['overlay_plugin']['status'] = $form_state['values']['overlay_config']['status'];
  $presets['overlay_plugin']['title'] = $form_state['values']['overlay_config']['title'];
  $presets['overlay_plugin']['message'] = $form_state['values']['overlay_config']['message'];
  $presets['overlay_plugin']['cookie_interval'] = $form_state['values']['overlay_config']['cookie_interval'];
  $presets['overlay_plugin']['page_delay'] = $form_state['values']['overlay_config']['page_delay'];
  $presets['overlay_plugin']['pester'] = $form_state['values']['overlay_config']['pester'];
  $presets['overlay_plugin']['developer_options'] = $form_state['values']['overlay_config']['developer_options'];
  $presets['overlay_plugin']['redirect_status'] = $form_state['values']['overlay_config']['redirect_status'];
  $presets['overlay_plugin']['redirect_page'] = $form_state['values']['overlay_config']['redirect_page'];
  $presets['overlay_plugin']['excluded_pages'] = $form_state['values']['excluded_pages'];
  $presets['overlay_plugin']['copy_config'] = $form_state['values']['copy_config'];
  $presets['overlay_plugin']['field_table'] = $form_state['values']['field_table'];
  $presets['overlay_plugin']['overlay_groups'] = $form_state['values']['overlay_groups'];

  // Check saved groups against aliases and delete older data accordingly.
  variable_set('kmsuite_object', json_encode($presets));
  drupal_set_message(t('Configuration settings saved.'), 'status', FALSE);
}
