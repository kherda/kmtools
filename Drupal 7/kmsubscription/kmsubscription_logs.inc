<?php

/**
 * @file
 * KM subscription submission log
 */

/**
 * Kmsuite recent logs.
 */
function kmsubscription_recent_logs() {

  $total = db_query("SELECT COUNT(action) as total, form, action, created FROM kmsubscription_data
                          GROUP BY form, action")->fetchAll();

  foreach ($total as $row) {

    $rows[] = array($row->total,
      ucfirst($row->form),
      $row->action,
    );

  }

  $header = array("Total", "Form", 'Action');

  $form['kmsuite']['totals'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recent Logs - Total'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="filter-div">',
    '#suffix' => '</div>',
  );

  $form['kmsuite']['totals']['filter'] = array(
    '#type' => 'date_popup',
    '#date_format' => 'Y-m-d',
  );

  $form['kmsuite']['totals']['filter']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Filter"),
    '#ajax' => array(
      'callback' => 'ajax_filter_callback',
      'wrapper' => 'filter-div',
      'method' => 'replace',
    ),
  );

  $form['kmsuite']['totals']['total'] = array(
    '#markup' => '<strong>Totals</strong> ' . theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'sticky' => TRUE)
    ),
  );

  $results = db_select('kmsubscription_data', 'kmdata')
    ->extend('PagerDefault')
    ->limit(50)
    ->fields('kmdata')
    ->orderBy('created', 'DESC')
    ->execute()
    ->fetchAll();

  $rows = array();
  foreach ($results as $row) {

    $rows[] = array($row->id,
      $row->group_id,
      ucfirst($row->form),
      $row->action,
      $row->email,
      $row->apiresponse,
      date('F d, Y H:i:s', $row->created),
    );
  }

  $header = array(
    "ID",
    "List ID",
    'Form',
    'Action',
    'Email',
    'API Response',
    'Created');

  $form['kmsuite']['all'] = array(
    '#markup' => '<strong>Recent log messages</strong> ' . theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'sticky' => TRUE
      )) . theme('pager'),
  );

  return $form;
}

/**
 * Logs Filter callback.
 *
 * @param array $form
 *   form array
 * @param array $form_state
 *   form submit array
 */
function ajax_filter_callback($form, $form_state) {

  $total = db_query("SELECT COUNT(action) as total, form, action, created FROM kmsubscription_data
             WHERE created  > " . strtotime($form_state['values']['filter']) . " AND created < " . time() . "
             GROUP BY form, action")->fetchAll();

  foreach ($total as $row) {

    $rows[] = array($row->total,
      $row->form,
      $row->action,
    );
  }

  $header = array("Total", "Form", 'Action');

  $form['kmsuite']['totals']['total'] = array(
    '#markup' => '<strong>Totals</strong> ' . theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'sticky' => TRUE)),
  );

  return $form['kmsuite']['totals'];

}
