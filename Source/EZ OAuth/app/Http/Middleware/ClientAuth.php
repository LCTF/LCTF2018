<?php

namespace App\Http\Middleware;

use Closure;
use Illuminate\Http\Request;
use Illuminate\Http\Response;
use Illuminate\Support\Facades\Cache;

class ClientAuth
{
    private static $client_pool;

    /**
     * Initialize all needed data.
     */
    public function __construct()
    {
        if (!Cache::has('client_pool')) Cache::forever('client_pool', []);
        self::$client_pool = (array) Cache::get('client_pool');
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
        if ($request->has('client_id')) {
            $client_id = $request->input('client_id');
            if (array_key_exists($client_id, self::$client_pool)) {
                $client_info = Cache::get('client_info_'.$client_id);
                $request->attributes->add(['client_info' => $client_info]);
            } else {
                return response()->json(['code'=>403, 'message'=>'Forbidden: invalid client id.']);
            }
        } else {
            return response()->json(['code'=>401, 'message'=>'Unauthorized: missing parameter.']);
        }
        return $next($request);
    }
}
