Bu uygulama, ESP32 WROOM kartına bağlı TFT ekranında ürün bilgilerini görüntüleyen ve wifi üzerinden bir web sunucusu aracılığıyla güncellenen bir sistemdir. Arduino, belirtilen WiFi ağına bağlanarak bir HTTP POST isteği ile web sunucusuna veri gönderir. Sunucu, "/update" yoluna gelen isteklerde JSON formatında ürün bilgilerini alır ve bu verilerle işlem yapar. Alınan veriler ürün barkodu, ürün ismi, fiyatı, indirim miktarı ve indirimli fiyatı gibi detayları içerir.

Veri başarılı bir şekilde alındığında, bu veriler seri portta görüntülenir ve tft ekrana aktarılır, seri portu dinleyerek veri akışını görüntüleyebilirsiniz. Kullanıcı barkod, ürün ismi, fiyat ve varsa indirim oranı gibi bilgileri ekranda görebilir. Sistem, bir indirim uygulanmışsa hem indirimsiz hem de indirimli fiyatı gösterecek şekilde tasarlanmıştır.

Veri alındığı her seferde ekran sıfırlanarak yeni bilgiler gösterilir ve uygulama, sunucudan gelen POST isteği beklerken sürekli olarak çalışmaya devam eder. Bu sayede ürün bilgileri dinamik olarak değiştirilebilir ve anında ekranda görüntülenebilir.

Pythonda geliştirdiğim ara programın veri yollama sırasında verdiği log mesajı:
![image](https://github.com/user-attachments/assets/a22c795b-78e8-4b7c-b1e4-bf68b5932d7a)

Arduino ide üzerinde serial montiorde portu dinlediğimizde dönen veri :
![image](https://github.com/user-attachments/assets/259487cc-a3dc-490a-a705-97dd4cbad37c)

etiketin fiziksel çıktısı:
![image](https://github.com/user-attachments/assets/595359f0-ddf1-4a5b-b4f9-fcd24c9d099b)

etiketin sağ üstünde bulunan yeşil kare wifi bağlantısı olup olmadığına göre belirir. eğer wifi bağlantısı yok ise kırmızı kare ile fiziksel olarak durumu döndürür. pythondan veri alındığı gibi fiziksel tft ekrana bu veri aktarılır veya güncellenmiş ise güncellenir.


