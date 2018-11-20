<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <link rel="icon" href="/asserts/favicon.png">
    <link rel="shortcut icon" href="/asserts/favicon.png">
    <title>Authentication</title>
    <link rel="stylesheet" href="//cdn.jsdelivr.net/npm/bootstrap@4.1.3/dist/css/bootstrap.min.css">
    <link rel="stylesheet" href="/asserts/css/auth-gateway.css">
  </head>

  <body class="text-center">
    <div class="card text-center">
      <div class="card-body">
        <h5 class="card-title">Authentication</h5>
        <p class="card-text">Login with following credential providers.</p>
        <a href="?provider=TYPCN_ACCOUNT" data-toggle="tooltip" data-placement="bottom" title="TYPCN Account"x>
            <img src="/asserts/img/typcn-logo-blue.png" alt="TYPCN Account" width="50" height="50">
        </a>
      </div>
    </div>
    <script src="//cdn.jsdelivr.net/npm/jquery-slim@3.0.0/dist/jquery.slim.min.js" crossorigin="anonymous"></script>
    <script src="//cdn.jsdelivr.net/npm/popper.js@1.14.4/dist/umd/popper.min.js" crossorigin="anonymous"></script>
    <script src="//cdn.jsdelivr.net/npm/bootstrap@4.1.3/dist/js/bootstrap.min.js" crossorigin="anonymous"></script>
    <script>
      $(function () {
        $('[data-toggle="tooltip"]').tooltip()
      })
    </script>
  </body>
</html>
