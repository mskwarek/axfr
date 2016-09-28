angular.module('Coder', ['myAppControllers', 'myAppServices'] );

angular.module('myAppControllers', [])
	.controller('codeController',
				['$scope',
				 'srvInfo',
				 function ($scope, srvInfo) {
					$scope.decode = function(data) {//run decoder
						var RE = /^[a-z|A-Z]*$/;
							if(document.getElementById('text_to_decode').value.match(RE)){
								srvInfo.getDecoded(
								function(data) {
									$scope.decoder_result = data;
								});
								document.getElementById('text_to_decode').style='background-color:white;';
								document.getElementById('text_error').style='display:none;';
							}else{
								document.getElementById('text_error').style='display:inline;';
								document.getElementById('text_to_decode').style='background-color:#ffa1a1;';
								document.getElementById('text_error').innerHTML="Put english chars only";
							}
					};
					$scope.code = function(data) {//run coder
						var RE = /^[a-z|A-Z]*$/;
							if(document.getElementById('text_to_code').value.match(RE)){
								srvInfo.getCoded(
								function(data) {
									$scope.coder_result = data;
								});
								document.getElementById('text_to_code').style='background-color:white;';
								document.getElementById('text_error').style='display:none;';
							}else{
								document.getElementById('text_error').style='display:inline;';
								document.getElementById('text_to_code').style='background-color:#ffa1a1;';
								document.getElementById('text_error').innerHTML="Put english chars only";
							}
					};
					$scope.cd = function(data) {//run when change key
						var RE = /^[0-9]+$/;
							if(document.getElementById('input_key').value.match(RE)){//check if key is number
								srvInfo.getCoded(//if so code and decode with the new key
								function(data) {
									$scope.coder_result = data;
								});
								srvInfo.getDecoded(
								function(data) {
									$scope.decoder_result = data;
								});
								document.getElementById('input_key').style='background-color:white;';
								document.getElementById('key_error').style='display:none;';
							}else{
								document.getElementById('key_error').style='display:inline;';
								document.getElementById('input_key').style='background-color:#ffa1a1;';
								document.getElementById('key_error').innerHTML="Put numbers here";
							}
					};
				 }]);

// communication with the server
angular.module('myAppServices', [])
    .service('srvInfo',
             function($http) {
                 this.getDecoded = function(callback) {
                     return $http.get('/ajax/coderpy/runDecoder/?text='+document.getElementById('text_to_decode').value+'&input_key='+document.getElementById('input_key').value).success(callback);
                 };
                 this.getCoded = function(callback) {
                     return $http.get('/ajax/coderpy/runCoder/?text='+document.getElementById('text_to_code').value+'&input_key='+document.getElementById('input_key').value).success(callback);
                 };
             });


