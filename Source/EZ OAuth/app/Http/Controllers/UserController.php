<?php

namespace App\Http\Controllers;

use App\Http\Controllers\Controller;
use Illuminate\Http\Request;
use Illuminate\Http\Response;

class UserController
{

    /**
     * Initialize all needed data.
     */
    public function __construct()
    {
    }

    /**
     * Handle isVerified API request.
     *
     * @param \Illuminate\Http\Request $request
     * @return Response
     */
    public function verified(Request $request)
    {
        if ($request->has('app_id') && $request->has('request_id') && $request->has('email') && $request->has('sign')) {
            if ($request->input('app_id') == 'oqEUfB6GyLysD9rK') {
                $sign = md5('app_id=oqEUfB6GyLysD9rK&request_id='.$request->input('request_id').'&email='.$request->input('email').'&app_secret=NCKhqJYmfYpCnVMTsPmU5dT2');
                if ($request->input('sign') == $sign) {
                    $sign = md5('app_id=oqEUfB6GyLysD9rK&request_id='.$request->input('request_id').'&email='.$request->input('email').'&app_secret=UZSt7NsR7DqMhcjP3YuYnmJW');
                    return response()->json(['code'=>200, 'result'=>false, 'sign'=>$sign]);
                } else {
                    return response()->json(['code'=>403, 'message'=>'Access Denied: invalid sign.']);
                }
            } else {
                return response()->json(['code'=>403, 'message'=>'Access Denied: invalid app_id.']);
            }
        } else {
            return response()->json(['code'=>400, 'message'=>'Bad Request: missing parameter.']);
        }
    }

    /**
     * Handle isAdmin API request.
     *
     * @param \Illuminate\Http\Request $request
     * @return Response
     */
    public function admin(Request $request)
    {
        if ($request->has('app_id') && $request->has('request_id') && $request->has('email') && $request->has('sign')) {
            if ($request->input('app_id') == 'oqEUfB6GyLysD9rK') {
                $sign = md5('app_id=oqEUfB6GyLysD9rK&request_id='.$request->input('request_id').'&email='.$request->input('email').'&app_secret=NCKhqJYmfYpCnVMTsPmU5dT2');
                if ($request->input('sign') == $sign) {
                    $sign = md5('app_id=oqEUfB6GyLysD9rK&request_id='.$request->input('request_id').'&email='.$request->input('email').'&app_secret=UZSt7NsR7DqMhcjP3YuYnmJW');
                    return response()->json(['code'=>200, 'result'=>false, 'sign'=>$sign]);
                } else {
                    return response()->json(['code'=>403, 'message'=>'Access Denied: invalid sign.']);
                }
            } else {
                return response()->json(['code'=>403, 'message'=>'Access Denied: invalid app_id.']);
            }
        } else {
            return response()->json(['code'=>400, 'message'=>'Bad Request: missing parameter.']);
        }
    }
 
}