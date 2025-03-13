const char UDC_HTML[] = R"=====(
<!DOCTYPE html><html>
<head>
    <title>Inicio de sesión - Servizos UDC - Servizo de Autenticación Centralizada</title>
    <meta charset="UTF-8"/><meta http-equiv="X-UA-Compatible" content="IE=edge"/><meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no"/><link rel="stylesheet" href="/webjars/font-awesome/5.0.13/web-fonts-with-css/css/fontawesome-all.min.css"/><link type="text/css" rel="stylesheet" href="/webjars/lato/0.3.0/css/lato.min.css"/><link rel="stylesheet" href="/css/cas.css?ver=2"/><link rel="icon" href="/favicon.ico" type="image/x-icon"/><script type="text/javascript" src="/webjars/jquery/3.3.1/jquery.min.js"></script>

<meta http-equiv="X-UA-Compatible" content="IE=edge"/><meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no"/></head>

<body class="login">
<header role="banner">
    </header>

<main role="main" class="container mt-3 mb-3">
    <div id="content" class="row">
        <div class="col-md mb-3">
            <div class="card" style="max-width: 26rem; margin:0px auto">
                <div class="card-header text-center">
	                <span class="d-none d-sm-block">
	                   <h2>Inicio de sesión</h2>
	                    <span class="fa-stack fa-1x">
	                      <span class="fa fa-circle fa-stack-2x fa-udc"></span>
	                      <span class="fa fa-lock fa-stack-1x fa-inverse"></span>
	                    </span>
                    </span>
                    <span class="fa-2x d-flex d-sm-none">
	                    <img id="udc_logo_cor" src="https://assets.udc.es/images/udc_logo_cor.png" title="Universidade da Coruña" alt="Logo da Universidade da Coruña" width="310" height="40"></span>
                </div>
                <div class="card-body">
                    <div id="list-providers" class="d-md-block bg-light">
                    <ul>
                        <li>
                            <a class="btn btn-udc btn-block btn-azuread"
                               href="clientredirect?client_name=Azure&amp;service=https://persoal.udc.es/epsilonnet/default.aspx"
                               title="Identifícate con usuario e contrasinal" autoredirect="false">

                                <!--  <span th:case="*" class="fa fa-sign-in-alt"></span>  --><span class="fa fa-user-shield"></span>
                                <span class="d-sm-none"> Usuario e contrasinal</span><span class="d-none d-sm-inline"> Identifícate con usuario e contrasinal</span>
                            </a>
                        </li>
                    </ul>
        </div>
                    <script type="text/javascript">
                        var i = "Un momento por favor..."
                        $( document ).ready(function() {
                            $("#fm1").submit(function () {
                                $(":submit").attr("disabled", true);
                                $(":submit").attr("value", i);
                                console.log(i);
                                return true;
                            });
                        });
                    </script>

                     <p>
					  <a href="https://cas-saml.udc.es:8443/login?service=https://persoal.udc.es/epsilonnet/default.aspx" class="btn btn-block btn-udc"><span class="fa fa-key"></span><span class="d-sm-none"> Certificado dixital</span><span class="d-none d-sm-inline"> Identifícate con certificado dixital</span></a>
					</p>
					
					<div>
                        </br>
                        <span><ul class="list-unstyled"><li title="Preme na ligazón se desexas recuperar o teu contrasinal"><span class="fa fa-unlock"></span><a href="https://servizos.udc.es/activacion/recuperar-contrasinal"> Esqueciches o teu contrasinal?</a></li><li title="Se é a primeira vez que nos visitas, debes activar a tua conta de servizos"><span class="fa fa-user-plus"></span><a href="https://servizos.udc.es/activacion"> Activa a túa conta de servizos</a></li><li title="Se tes algún problema neste enderezo terás acceso ao portal de axuda dos servizos telemáticos da Universidade da Coruña"><span class="far fa-question-circle"></span><a href="https://axudatic.udc.gal/"> Necesitas axuda?</a></li></ul></span>
                        <p/></div>
                    
                </div>
            </div>
        </div>
        <div id="notices" class="col-md">
            <!--<div th:replace="fragments/insecure :: insecure"><a href="fragments/insecure.html">insecure alert goes
                here</a></div>--><!-- <div th:replace="fragments/defaultauthn :: staticAuthentication">
                <a href="fragments/defaultauthn.html">defaultAuthn</a>
                fragment
            </div> --><div id="cookiesDisabled" class="card" style="display: none;">
    <div class="card-body">
        <h5 class="card-title">Browser cookies disabled</h5>
        <p class="card-text">Your browser does not accept cookies. Single Sign On WILL NOT WORK.</p>
    </div>
</div>
            <!--<div th:replace="fragments/serviceui :: serviceUI"><a href="fragments/serviceui.html">service ui</a> fragment</div>--><!--<div th:replace="fragments/loginProviders :: loginProviders"><a href="fragments/loginProviders.html">loginProviders</a>
                fragment
            </div>--><!--<div th:replace="fragments/cas-resources-list :: cas-resource-list">
                <a href="fragments/cas-resources-list.html">cas-resource</a> list fragment
            </div>--><div >


<div class="card card-grey d-none d-sm-block" style="max-width: 26rem; margin:0px auto">
 
<script language="javascript">
<!--
function makeArray(n) {
this.length = n;
for (var i=1; i<=n; i++) {
this[i] = "";
}
return this;
}
var i;
function imagenaleatoria() {
var i = Math.floor(12 * Math.random() + 1);
i=0;
return Imagenes[i];
}
Imagenes = new makeArray(13);
Imagenes[0]="https://cas-saml.udc.es/images/udc-000.png";
Imagenes[1]="https://cas-saml.udc.es/images/udc-001.png";
Imagenes[2]="https://cas-saml.udc.es/images/udc-002.png";
Imagenes[3]="https://cas-saml.udc.es/images/udc-003.png";
Imagenes[4]="https://cas-saml.udc.es/images/udc-004.png";
Imagenes[5]="https://cas-saml.udc.es/images/udc-005.png";
Imagenes[6]="https://cas-saml.udc.es/images/udc-006.png";
Imagenes[7]="https://cas-saml.udc.es/images/udc-007.png";
Imagenes[8]="https://cas-saml.udc.es/images/udc-008.png";
Imagenes[9]="https://cas-saml.udc.es/images/udc-009.png";
Imagenes[10]="https://cas-saml.udc.es/images/udc-010.png";
Imagenes[11]="https://cas-saml.udc.es/images/udc-011.png";
Imagenes[12]="https://cas-saml.udc.es/images/udc-012.png";
// -->
</script>

<SCRIPT language="javascript">document.write("<a href='https://servizos.udc.es/activacion'><IMG SRC=" +imagenaleatoria() +" BORDER=0 class='card-img'></a>");</SCRIPT>

 </div>
 <div class="card card-grey d-none d-sm-block" style="font-size: 0.95em; padding-left: 15px;padding-right: 15px;padding-top:1em;padding-bottom:1em;max-width: 26rem;margin:15.2px auto">
  Por motivos de seguridade, pecha a sesión e pecha o teu navegador web cando remates de acceder aos servizos que requiren autenticación.
</div>
</div>			
			<div >

</div>
        </div>
    </div>
</main>

<script type="text/javascript" src="/webjars/zxcvbn/4.3.0/zxcvbn.js"></script>
<script type="text/javascript" src="/webjars/jquery-ui/1.12.1/jquery-ui.min.js"></script>
<script type="text/javascript" src="/webjars/jquery-cookie/1.4.1-1/jquery.cookie.js"></script>
<script src="/webjars/bootstrap/4.1.0/js/bootstrap.bundle.min.js"></script>

<script src="/webjars/headjs/1.0.3/head.min.js"></script>
<script src="/webjars/store.js/1.3.17/store.min.js"></script>
<script type="text/javascript" src="/js/cas.js"></script>

<script>
head.ready(document, function () {
    if (!window.jQuery) {
    	var jqueryUrl = "\/webjars\/jquery\/3.3.1\/jquery.min.js"; 
        head.load(jqueryUrl, loadjQueryUI);
    } else {
        notifyResourcesAreLoaded(resourceLoadedSuccessfully);
    }
});

function loadjQueryUI() {
	var jqueryUrl = "\/webjars\/jquery-ui\/1.12.1\/jquery-ui.min.js"; 
	head.load(jqueryUrl, loadjQueryCookies);
}

function loadjQueryCookies() {
	var jqueryUrl = "\/webjars\/jquery-cookie\/1.4.1-1\/jquery.cookie.js"; 
	head.load(jqueryUrl, notifyResourcesAreLoaded(resourceLoadedSuccessfully));
}

function notifyResourcesAreLoaded(callback) {
    if (typeof callback === "function") {
        callback();
    }
}
</script>

<script>
    /*<![CDATA[*/

    var trackGeoLocation = false;

    var googleAnalyticsTrackingId = null;

    if (googleAnalyticsTrackingId != null && googleAnalyticsTrackingId != '') {
        (function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
            (i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
            m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
        })(window, document, 'script', 'https://www.google-analytics.com/analytics.js', 'ga');

        ga('create', googleAnalyticsTrackingId, 'auto');
        ga('send', 'pageview');
    }

    /*]]>*/
</script>

</body>
</html>
)=====";
