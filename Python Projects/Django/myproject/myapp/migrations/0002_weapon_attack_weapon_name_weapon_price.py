# Generated by Django 5.0.1 on 2024-01-22 14:39

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('myapp', '0001_initial'),
    ]

    operations = [
        migrations.AddField(
            model_name='weapon',
            name='attack',
            field=models.IntegerField(null=True),
        ),
        migrations.AddField(
            model_name='weapon',
            name='name',
            field=models.CharField(default='Weapon', max_length=100),
        ),
        migrations.AddField(
            model_name='weapon',
            name='price',
            field=models.IntegerField(null=True),
        ),
    ]
