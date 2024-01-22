from django.shortcuts import render
from django.http import HttpResponse
from .models import Weapon

# Create your views here.
def index(request):
    weapons = Weapon.objects.all()
    
    return render(request, 'index.html', {'weapons': weapons})

def counter(request):
    text = request.POST['text']
    amount_of_words = len(text.split())
    return render(request, 'counter.html', {'amount': amount_of_words})