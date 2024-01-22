from django.db import models

# Create your models here.
class Weapon(models.Model):
    name = models.CharField(max_length=100, default="Weapon")
    attack = models.IntegerField(null=True)
    price = models.IntegerField(null=True)
    