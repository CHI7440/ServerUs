// FIXING NAVBAR

window.onscroll = function(){myFunction()};

  var navigationBar = document.querySelector("nav");

  var sticky = navigationBar.offsetTop;

  function myFunction() {
    if (window.pageYOffset >= sticky) {
      navigationBar.classList.add("sticky");
    } else {
      navigationBar.classList.remove("sticky");
    }
  }

  $(".allSkills").addClass("active")
  $(".allSkills .skill .skill-bar span").each(function() {
     $(this).animate({
        "width": $(this).parent().attr("data-bar") + "%"
     }, 1000);
     $(this).append('<b>' + $(this).parent().attr("data-bar") + '%</b>');
  });
  setTimeout(function() {
     $(".allSkills .skill .skill-bar span b").animate({"opacity":"1"},1000);
  }, 2000);