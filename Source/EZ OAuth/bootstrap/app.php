<?php

require_once __DIR__.'/../vendor/autoload.php';

try {
    (new Dotenv\Dotenv(__DIR__.'/../'))->load();
} catch (Dotenv\Exception\InvalidPathException $e) {
    //
}

/*
|--------------------------------------------------------------------------
| Create The Application
|--------------------------------------------------------------------------
|
| Here we will load the environment and create the application instance
| that serves as the central piece of this framework. We'll use this
| application as an "IoC" container and router for this framework.
|
*/

$app = new Laravel\Lumen\Application(
    realpath(__DIR__.'/../')
);

$app->withFacades();

$app->withEloquent();

// Add required alias.
$app->alias('cookie', \Illuminate\Cookie\CookieJar::class);
$app->alias('cookie', \Illuminate\Contracts\Cookie\Factory::class);
$app->alias('cookie', \Illuminate\Contracts\Cookie\QueueingFactory::class);
$app->alias('session', \Illuminate\Session\SessionManager::class);
$app->alias('session.store', \Illuminate\Session\Store::class);
$app->alias('session.store', \Illuminate\Contracts\Session\Session::class);

// Read the configuration file.
// You need to prepare a configuration file similar to Laravel in config/session.php.
$app->configure('session');
$app->configure('app');

$app->withFacades(true, [
    \Illuminate\Support\Facades\Config::class => 'Config'
]);

// Set cookie, Session service provider.
$app->register(\Illuminate\Cookie\CookieServiceProvider::class);
$app->register(\Illuminate\Session\SessionServiceProvider::class);

// Set middleware.
$app->middleware([
    \Illuminate\Cookie\Middleware\EncryptCookies::class,
    \Illuminate\Cookie\Middleware\AddQueuedCookiesToResponse::class,
    \Illuminate\Session\Middleware\StartSession::class,
    \Illuminate\View\Middleware\ShareErrorsFromSession::class,
]);
/*
|--------------------------------------------------------------------------
| Register Class Alias
|--------------------------------------------------------------------------
|
| I don't know what to write here, sorry.
|
*/

class_alias('Ixudra\Curl\Facades\Curl', 'Curl');

/*
|--------------------------------------------------------------------------
| Register Container Bindings
|--------------------------------------------------------------------------
|
| Now we will register a few bindings in the service container. We will
| register the exception handler and the console kernel. You may add
| your own bindings here if you like or you can make another file.
|
*/

$app->singleton(
    Illuminate\Contracts\Http\Kernel::class,
    App\Http\Kernel::class
);

$app->singleton(
    Illuminate\Contracts\Debug\ExceptionHandler::class,
    App\Exceptions\Handler::class
);

$app->singleton(
    Illuminate\Contracts\Console\Kernel::class,
    App\Console\Kernel::class
);

$app->singleton(
    Illuminate\Redis\RedisServiceProvider::class,
    App\Cache\Redis::class
);

/*
|--------------------------------------------------------------------------
| Register Middleware
|--------------------------------------------------------------------------
|
| Next, we will register the middleware with the application. These can
| be global middleware that run before and after each request into a
| route or middleware that'll be assigned to some specific routes.
|
*/

$app->routeMiddleware([
    'AdminAuth' => App\Http\Middleware\AdminAuth::class,
    'ClientAuth' => App\Http\Middleware\ClientAuth::class,
    'WebAuth' => App\Http\Middleware\WebAuth::class,
]);

/*
|--------------------------------------------------------------------------
| Register Service Providers
|--------------------------------------------------------------------------
|
| Here we will register all of the application's service providers which
| are used to bind services into the container. Service providers are
| totally optional, so you are not required to uncomment this line.
|
*/

// $app->register(App\Providers\AppServiceProvider::class);
// $app->register(App\Providers\AuthServiceProvider::class);
// $app->register(App\Providers\EventServiceProvider::class);
$app->register(Illuminate\Redis\RedisServiceProvider::class);
$app->register(Ixudra\Curl\CurlServiceProvider::class);

/*
|--------------------------------------------------------------------------
| Load The Application Routes
|--------------------------------------------------------------------------
|
| Next we will include the routes file so that they can all be added to
| the application. This will provide all of the URLs the application
| can respond to, as well as the controllers that may handle them.
|
*/

$app->router->group([
    'namespace' => 'App\Http\Controllers',
], function ($router) {
    require __DIR__.'/../routes/web.php';
});

return $app;
