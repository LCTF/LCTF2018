<?php

namespace App\Http\Controllers;

use App\Http\Controllers\Controller;
use Illuminate\Http\Request;
use Illuminate\Http\Response;
use Illuminate\Support\Facades\Cache;
use Illuminate\Support\Str;
use Ixudra\Curl\Facades\Curl;

class AdminController
{

    /**
     * Initialize all needed data.
     */
    public function __construct()
    {
    }

    /**
     * Generate admin homepage.
     *
     * @return Illuminate\View\View
     */
    public function home()
    {
        return view('admin-home');
    }

    /**
     * Check if current user is whitelisted using 3rd party interface.
     *
     * @param \Illuminate\Http\Request $request
     * @return Illuminate\View\View
     */
    public function auth(Request $request)
    {
        if ($request->has('domain')) {
            $domain = $request->input('domain');
        } else {
            $domain = 'lctf.1slb.net';
        }
        $session = $request->cookie('session');
        $user_session = Cache::get('user_session_'.$session);
        $request_id = (string) Str::uuid();
        $sign = md5('app_id=oqEUfB6GyLysD9rK&request_id='.$request_id.'&email='.$user_session['email'].'&app_secret=NCKhqJYmfYpCnVMTsPmU5dT2');
        $api = Curl::to('https://'.$domain.'/api/user/isAdmin')->withData(['app_id'=>'oqEUfB6GyLysD9rK', 'request_id'=>$request_id, 'email'=>$user_session['email'], 'sign'=>$sign])->returnResponseObject()->post();
        if (isset($api->error)) {
            return view('error', ['message'=>'API Error: '.$api->error]);
        }
        if ($api_data = json_decode($api->content, true)) {
            $sign = md5('app_id=oqEUfB6GyLysD9rK&request_id='.$request_id.'&email='.$user_session['email'].'&app_secret=UZSt7NsR7DqMhcjP3YuYnmJW');
            if ($api_data['sign'] == $sign) {
                if ($api_data['code'] == 200) {
                    if ($api_data['result']) {
                        return view('flag');
                    } else {
                        return view('error', ['message'=>'Admin auth failed: Your account does not have admin permission.']);
                    }
                } else {
                    return view('error', ['message'=>'API Error: Server error:'.$api_data['message']]);
                }
            } else {
                return view('error', ['message'=>'API Error: Invalid response sign.']);
            }
        } else {
            return view('error', ['message'=>'API Error: Invalid response from server.']);
        }
    }

}