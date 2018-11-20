<?php

/*
|--------------------------------------------------------------------------
| Application Routes
|--------------------------------------------------------------------------
|
| Here is where you can register all of the routes for an application.
| It is a breeze. Simply tell Lumen the URIs it should respond to
| and give it the Closure to call when that URI is requested.
|
*/

$router->get('/', function () use ($router) {
    return view('home');
});

$router->group(['prefix' => 'api'], function () use ($router) {
    $router->group(['prefix' => 'user'], function () use ($router) {
        $router->post('/isVerified', 'UserController@verified');
        $router->post('/isAdmin', 'UserController@admin');
    });
});
$router->group(['prefix' => 'auth'], function () use ($router) {
    $router->get('/gateway', 'AuthController@gateway');
    $router->get('/callback/typcn_account', 'AuthController@callbackTYPCNAccount');
});
$router->group(['prefix' => 'user', 'middleware' => 'WebAuth'], function () use ($router) {
    $router->get('/', 'WebController@home');
    $router->post('/check', 'WebController@check');
    $router->post('/add', 'WebController@add');
});
$router->group(['prefix' => 'admin', 'middleware' => 'AdminAuth'], function () use ($router) {
    $router->get('/', 'AdminController@home');
    $router->post('/auth', 'AdminController@auth');
});
