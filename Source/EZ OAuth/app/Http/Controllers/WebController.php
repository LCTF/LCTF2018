<?php

namespace App\Http\Controllers;

use App\Http\Controllers\Controller;
use Illuminate\Http\Request;
use Illuminate\Http\Response;
use Illuminate\Support\Facades\Cache;
use Illuminate\Support\Str;
use Ixudra\Curl\Facades\Curl;

class WebController
{

    /**
     * Initialize all needed data.
     */
    public function __construct()
    {
    }

    /**
     * Generate user homepage.
     *
     * @return Illuminate\View\View
     */
    public function home()
    {
        return view('web-home');
    }

    /**
     * Check if a user is whitelisted using 3rd party interface.
     *
     * @param \Illuminate\Http\Request $request
     * @return Illuminate\View\View
     */
    public function check(Request $request)
    {
        if ($request->has('email')) {
            if ($request->has('domain')) {
                $domain = $request->input('domain');
            } else {
                $domain = 'lctf.1slb.net';
            }
            $pattern = '/^([0-9A-Za-z\\-_\\.]+)@([0-9a-z]+\\.[a-z]{2,3}(\\.[a-z]{2})?)$/i';
            if (preg_match($pattern, $request->input('email'))) {
                $request_id = (string) Str::uuid();
                $sign = md5('app_id=oqEUfB6GyLysD9rK&request_id='.$request_id.'&email='.$request->input('email').'&app_secret=NCKhqJYmfYpCnVMTsPmU5dT2');
                $api = Curl::to('https://'.$domain.'/api/user/isVerified')->withData(['app_id'=>'oqEUfB6GyLysD9rK', 'request_id'=>$request_id, 'email'=>$request->input('email'), 'sign'=>$sign])->returnResponseObject()->post();
                if (isset($api->error)) {
                    return view('error', ['message'=>'API Error: '.$api->error]);
                }
                if ($api_data = json_decode($api->content, true)) {
                    $sign = md5('app_id=oqEUfB6GyLysD9rK&request_id='.$request_id.'&email='.$request->input('email').'&app_secret=UZSt7NsR7DqMhcjP3YuYnmJW');
                    if ($api_data['sign'] == $sign) {
                        if ($api_data['code'] == 200) {
                            if ($api_data['result']) {
                                return view('web-user-info', ['email'=>$request->input('email'), 'status'=>'Verified']);
                            } else {
                                return view('web-user-info', ['email'=>$request->input('email'), 'status'=>'Unregistered']);
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
            } else {
                return view('error', ['message'=>'Please enter a valid email address.']);
            }
        } else {
            return view('error', ['message'=>'Invalid Request: missing parameter.']);
        }
    }

}