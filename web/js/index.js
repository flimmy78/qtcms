var oLeft,oBottom,oView,oPreView;
var	nViewNum = 0;
	$(function(){
		oLeft = $('#search_device');
		oBottom = $('#operating');
		oView = $('#playback_view');
		oPreView= $('#previewWindows')[0];
		setViewMod('div2_2');

		var oAs = $('ul.dev_list_btn a');
		var oDiv = $('div.dev_list');
	    
		$(window).off();

	    $('ul.filetree').treeview().find('span.file').click(function(){
			$(this).toggleClass('file_1')
		});
		oDiv.eq(1).hide();

		$('.hover').each(function(){
			var action = $(this).attr('class').split(' ')[0];
			addMouseStyle($(this),action);
		})
		
		oAs.each(function(index){
			$(this).click(function(){
				$(window).off();
				oAs.removeClass('active');
				oDiv.hide();
				$(this).addClass('active');
				oDiv.eq(index).show();
			})
		})
		ViewMax('preview');
		$('body')[0].onresize=function(){
			ViewMax('preview');
		}
		var num=0;
		$('div.dev_list span.channel').each(function(){ 
			$(this).click(function(){
				var devData = $(this).parent('li').parent('ul').prev('span.device').data('data');
				var chlData = $(this).data('data');
				alert(num)
				var wind = num;
				num++;
				for(i in chlData){ 
					devData[i]=chlData[i];
				}
				show(devData);
				oPreView.OpenCameraInWnd(wind,devData.address,devData.port,devData.eseeid,chlData.channel_number,chlData.stream_id,devData.username,devData.password,chlData.channel_name,devData.vendor);
			})
		})
	})
	//获取当前窗口最经一个可用的窗口。
	function getWind(i){
		if(oPreView.GetWindowConnectionStatus(i)!=0){
			i++;
			return oPreView.GetWindowConnectionStatus(i);
		}else{ 
			return i;
		}
	}
	function setViewMod(i){
		oPreView.setDivMode(i);
	}