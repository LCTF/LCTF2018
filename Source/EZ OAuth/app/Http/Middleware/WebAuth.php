<?php

namespace App\Http\Middleware;

use Closure;
use Illuminate\Http\Request;
use Illuminate\Http\Response;
use Illuminate\Support\Facades\Cache;

class WebAuth
{

    /**
     * Initialize all needed data.
     */
    public function __construct()
    {
    }

    /**
     * Handle an incoming request.
     *
     * @param \Illuminate\Http\Request $request
     * @param \Closure $next
     * @return mixed
     */
    public function handle(Request $request, Closure $next)
    {
        if ($session = $request->cookie('session')) {
            if (Cache::has('user_session_'.$session)) {
                $user_session = Cache::get('user_session_'.$session);
                $host = substr($user_session['email'], strrpos($user_session['email'], '@') + 1);
                if (stripos($host, 'pwnhub.cn') !== false) {
                    return $next($request);
                } else {
                    return view('auth-not-whitelisted');
                }
            } else {
                return view('error', ['message'=>'User Gateway: You have an invalid session on your browser. Please clear all cookies of this site and try again. If you think this is a bug, please contact the hostmaster.']);
            }
        } else {
            return redirect('auth/gateway');
        }
    }

}
