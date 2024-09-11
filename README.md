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

NOT : Bu proje önceden yazdığım restful api, api consumer, nodejs urun ve yonetim sitesi ve python ile esp32 ye veri yollama programı ile birlikte kusursuz çalışmaktadır. proje işleyişi hakkında dökümantasyonumda işleyişi anlattığım yazıyı inceleyebilirsiniz. 

Proje İşleyişi
1. Merkezi API Sistemi(borauysl/urun-.net-core-rest-api)
Ana merkez, fiyat güncellemeleri için bir API sistemi üzerinden verileri sağlayacaktır. Bu API, sürekli olarak güncellenerek veritabanındaki en güncel fiyatları mağazalara iletecektir.
2. API Consumer Hizmeti(borauysl/urun-api-consumer)
Mağazalarda kullanılacak API Consumer hizmeti, merkezi API'den belirlenen aralıklarla verileri alarak mağaza içerisindeki SQL veritabanında işleyecektir. Bu hizmet, mağaza içinde lokal veya sunucu üzerinde çalışabilecek şekilde tasarlanacak ve verilerin sürekli güncel kalmasını sağlayacaktır.
3. Otomasyon Uygulaması(borauysl/nodejs-otomatik-sql-girisli-urun-ve-etiket-yonetimi)
Mağaza personelinin, SQL bilgisine ihtiyaç duymadan fiyat güncellemelerini ve etiket yönetimini kolaylıkla yapabilmesi için bir otomasyon uygulaması sunulacaktır. Bu uygulama, aşağıdaki özelliklere sahip olacaktır:
•	Güncel fiyat verilerini görüntüleme,
•	Ürün barkodlarını etiketlere tanımlama,
•	Etiket IP adreslerini düzenleme,
•	Yeni etiket ekleme veya var olan etiketleri silme.
4. Ara Katman Yazılımı ve Mikrodenetleyici Entegrasyonu (borauysl/esp32-wifi-uzerinden-post-ile-sunucuya-veri-yollama)
API Consumer'dan elde edilen veriler, Wi-Fi modül destekli mikrodenetleyicilere iletilecek. Bu süreçte ara program, etiketlerin kimlik bilgilerine göre ürün fiyatlarını dağıtacak ve belirtilen süre zarfında etiket ekranlarını güncelleyecektir. Bu sistem hem local ağ üzerinden hem de sunucu üzerinde çalışacak şekilde esnek tasarlanacaktır.
5. Elektronik Etiket ve Mikrodenetleyici Sistemi(Mevcut proje)
Elektronik etiketler, pil destekli mikrodenetleyicilerle çalışacak ve ayda bir pil değişimi yapıldığı sürece kesintisiz olarak çalışmaya devam edecektir. Bu sistem, raflardaki fiyatları güncel tutarken personel iş gücüne minimum düzeyde ihtiyaç duyulmasını sağlayacaktır.



