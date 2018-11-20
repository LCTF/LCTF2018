# LCTF2018 - EZ OAuth

This is the source codebase of the challenge EZ OAuth.

## Requirements

- Nginx / Apache
- PHP >= 7.1
- A CGI for communicating between webserver and PHP (use PHP-FPM for Nginx)
- composer (Ubuntu users can directly use `apt install composer` to install)
- Following PHP plugins: `php-curl` `php-json` `php-mbstring` `php-xml` `php-redis`
- Redis (Ubuntu users can directly use `apt install redis-server` to install)

## Installation

```bash
git pull git@github.com:LCTF/LCTF2018.git
mv LCTF2018/Source/EZ\ OAuth /var/www/ez_oauth
cd /var/www/ez_oauth
composer install
cd ..
chown -R www-data:www-data ez_oauth
```

Note: Change the path (`/var/www/ez_oauth`) and webserver & CGI username (`www-data`) to your system environments.

## Configuration

### Application Config

Copy `.env.example` to `.env` , and edit the items in it.

If you want to deploy it on an online environment, you should generate a new `APP_KEY` and replace the one given.

`TYPCN_APPID` and `TYPCN_SECRET` is the application's Appkey and AppSecret of TYPCN Account. You should apply for your own ones and insert it here, otherwise the OAuth function will not work.

Fields start with `DB_` can be ignored.

Fields start with `REDIS_` should match the configuration of your local redis server.

Do not change other values or delete any items.

### Webserver Config

Configuration of your webserver should be same with apps using [Laravel Framework](https://laravel.com/docs/5.7#web-server-configuration).

For people using Nginx & PHP-FPM, here's a sample site config clip:

```nginx
server {

    listen 443;

    server_name lctf.1slb.net;

    ssl on;
    ssl_certificate cert/star_1slb_net.crt;
    ssl_certificate_key cert/star_1slb_net.key;
    ssl_protocols TLSv1.2;
    ssl_ciphers 'CHACHA20:-DHE:AESGCM:AESCCM+ECDH:-SHA256:AES:+AES256:+DHE:+AESCCM8:+SHA256:-SHA384:+SHA:-RSA:-ECDH+ECDSA+SHA:!DSS:!PSK:!aNULL:!SRP:!aECDH';
    ssl_prefer_server_ciphers on;
    root /var/www/ez_oauth/public;

  location / {
    index index.html index.php;
    if (!-e $request_filename) {
      rewrite ^/(.*)$ /index.php/$1 last;
      break;
    }
  }

# use socket to call php-fpm
  location ~ .+\.php($|/) {
    add_header Cache-Control private;
    fastcgi_pass unix:/run/php/php7.2-fpm.sock;
    include snippets/fastcgi-php.conf;
  }

}
```

## License


This application is open-sourced software licensed under the [MIT license](http://opensource.org/licenses/MIT).