<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Http\Response;
use Illuminate\Support\Facades\Cache;
use Illuminate\Support\Str;
use Ixudra\Curl\Facades\Curl;

class AuthController
{

    private static $OAUTH_TYPCN_ACCOUNT = NULL;

    /**
     * Initialize all needed data.
     */
    public function __construct()
    {
        self::$OAUTH_TYPCN_ACCOUNT = config('app.OAUTH.TYPCN_ACCOUNT');
    }

    /**
     * User Gateway
     *
     * @param Request $request
     * @return mixed
     */
    public function gateway(Request $request)
    {
        if ($session = $request->cookie('session')) {
            if (Cache::has('user_session_'.$session)) {
                return redirect('/')->withCookie('session', '', 0, '/', 'job.indrasub.org', true, true);
            } else {
                return view('error', ['message'=>'User Gateway: You have an invalid session on your browser. Please clear all cookies of this site and try again. If you think this is a bug, please contact the hostmaster.']);
            }
        } else {
            if ($request->has('provider')) {
                switch ($request->input('provider')) {
                    case 'TYPCN_ACCOUNT':
                        return redirect()->to(self::$OAUTH_TYPCN_ACCOUNT['URI_PASSPORT'].self::$OAUTH_TYPCN_ACCOUNT['APPID']);
                    default:
                        return view('error', ['message'=>'User Gateway: Unsupported authentication provider. If you think this is a bug, please contact the hostmaster.']);
                }
            } else {
                return view('auth-gateway');
            }
        }
    }

    /**
     * TYPCNAccount Auth Provider
     *
     * @param Request $request
     * @return mixed
     */
    public function callbackTYPCNAccount(Request $request)
    {
        if ($request->has('code')) {
            $token_request = Curl::to(self::$OAUTH_TYPCN_ACCOUNT['URI_ACCESSTOKEN'])->withData(['appid'=>self::$OAUTH_TYPCN_ACCOUNT['APPID'], 'appsecret'=>self::$OAUTH_TYPCN_ACCOUNT['SECRET'], 'code'=>$request->input('code')])->asJsonRequest()->returnResponseObject()->post();
            if ($token_data = json_decode($token_request->content, true)) {
                if ($token_data['code'] == 0) {
                    $user_request = Curl::to(self::$OAUTH_TYPCN_ACCOUNT['URI_USERINFO'])->withHeader('Authorization: '.$token_data['access_token'])->withHeader('User-Agent: MisakaNetwork/0.0.1')->returnResponseObject()->get();
                    if ($user_data = json_decode($user_request->content, true)) {
                        if ($user_data['code'] == 0) {
                            if ($user_data['data']['scope'][0] == 'user:email:force') {
                                if (isset($user_data['data']['user']['email'])) {
                                    $username = (string) Str::uuid();
                                    $session_id = (string) Str::uuid();
                                    if (isset($user_data['data']['user']['name'])) {
                                        Cache::put('user_session_'.$session_id, ['name'=>$user_data['data']['user']['name'], 'email'=>$user_data['data']['user']['email']], 61);
                                    } else {
                                        Cache::put('user_session_'.$session_id, ['name'=>$username, 'email'=>$user_data['data']['user']['email']], 61);
                                    }
                                    return redirect('/')->withCookie(cookie('session', $session_id, 60, '/', 'lctf.1slb.net', true, true));
                                } else {
                                    return view('error', ['message'=>'TYPCNAccount Auth Provider: Your user info does NOT contain account email address, please bind an email address to your account and try again.']);
                                }
                            } else {
                                return view('error', ['message'=>'TYPCNAccount Auth Provider: Permission scope mismatch. Do not try to modify requests, we need all the required permissions.']);
                            }
                        } else {
                            return view('error', ['message'=>'TYPCNAccount Auth Provider: Remote server returned an error. REQUEST=GET_USER_INFO | CODE='.$user_data['code']]);
                        }
                    } else {
                        return view('error', ['message'=>'TYPCNAccount Auth Provider: Remote server returned an invalid response. REQUEST=GET_USER_INFO']);
                    }
                } else {
                    return view('error', ['message'=>'TYPCNAccount Auth Provider: Remote server returned an error. REQUEST=GET_ACCESS_TOKEN | CODE='.$token_data['code']]);
                }
            } else {
                return view('error', ['message'=>'TYPCNAccount Auth Provider: Remote server returned an invalid response. REQUEST=GET_ACCESS_TOKEN']);
            }
        } else {
            if ($request->has('error')) {
                switch ($request->input('error')) {
                    case 'user_reject':
                        return view('error', ['message'=>'You rejected our request to access your account information. We can not authorize you without these basic information.']);
                    default:
                        return view('error', ['message'=>'TYPCNAccount Auth Provider: Remote server returned an error. | ERROR='.$request->input('error')]);
                }
            } else {
                return view('error', ['message'=>'TYPCNAccount Auth Provider: Invalid request.']);
            }
        }
    }

}

